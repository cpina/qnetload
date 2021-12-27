#include "networkinformationreader.h"

#include "utils.h"

#include <QFile>
#include <QNetworkInterface>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

/*
 * Copyright 2017, 2021 Carles Pina i Estany <carles@pina.cat>
 * This file is part of qnetload.
 *
 * qnetload is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qnetload is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qnetload.  If not, see <http://www.gnu.org/licenses/>.
 */

NetworkInformationReader::NetworkInformationReader(QObject* parent)
    : QObject(parent)
    , m_procNetDev("/proc/net/dev")
{
}

NetworkInformationReader::NetworkInformationReader(const QString& interfaceName,
                                                   const QString& procNetDevFile,
                                                   QObject* parent)
    : QObject(parent)
    , m_procNetDev(procNetDevFile)
{
    setInterfaceName(interfaceName);
}

QString NetworkInformationReader::ip() const
{
    QNetworkInterface interface = QNetworkInterface::interfaceFromName(interfaceName());

    QList<QNetworkAddressEntry> addresses = interface.addressEntries();

    QStringList result;

    Q_FOREACH(const QNetworkAddressEntry& address, addresses)
    {
        if (address.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            result.append(address.ip().toString());
        }
    }

    if (result.count() == 0)
    {
        return "-";
    }
    else
    {
        return result.join("-");
    }
}

void NetworkInformationReader::setInterfaceName(const QString& interfaceName)
{
    if (interfaceName != m_interfaceName)
    {
        m_interfaceName = interfaceName;
        emit interfaceNameChanged();
    }
}

QString NetworkInformationReader::procNetDev() const
{
    return m_procNetDev;
}

QString NetworkInformationReader::interfaceName() const
{
    return m_interfaceName;
}

QStringList NetworkInformationReader::listOfInterfaces() const
{
    QFile file(procNetDev());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QStringList();
    }

    QStringList listOfInterfaces;
    QString line;
    do
    {
        line = file.readLine();
        QRegularExpressionMatch match = parseLine(line);

        if (match.hasMatch())
        {
            listOfInterfaces.append(match.captured(1));
        }

    } while (!line.isEmpty());

    listOfInterfaces.sort();
    return listOfInterfaces;
}

NetworkInformationReader::NetworkBytesInOut NetworkInformationReader::readProcNetDevInterface(const QString& interface) const
{
    QFile file(procNetDev());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return NetworkInformationReader::NetworkBytesInOut();
    }

    QString line;
    do
    {
        line = file.readLine();

        QRegularExpressionMatch match = parseLine(line);

        if (match.hasMatch() && match.captured(1) == interface)
        {
            NetworkInformationReader::NetworkBytesInOut information;
            information.in = match.captured(2).toULong();
            information.out = match.captured(3).toULong();
            information.milliSecondsSinceEpoch = Utils::currentMSecsSinceEpoch();

            return information;
        }

    } while(!line.isEmpty());

    return NetworkInformationReader::NetworkBytesInOut();
}

QRegularExpressionMatch NetworkInformationReader::parseLine(const QString& line)
{
    QRegularExpression lineInformationRe(" *(.+): * (\\d+) +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +(\\d+).*");

    QRegularExpressionMatch match = lineInformationRe.match(line);

    return match;
}

NetworkInformationReader::NetworkBytesInOut NetworkInformationReader::readInformation() const
{
    NetworkInformationReader::NetworkBytesInOut information = readProcNetDevInterface(m_interfaceName);
    return information;
}

QString NetworkInformationReader::chooseInterfaceFromProcNetDevInterface() const
{
    // Choose a valid interface name from a priority list
    QStringList prefixes;

    prefixes.append("wl");      // wlan0 (WirelessLAN, classic) or wlp4s0 (WireLess Pci 4... systemd/udev)
    prefixes.append("enp");     // enp0s20f0u3c2 (EtherNet Pci 0 Slot 2...  systemd/udev)
    prefixes.append("eth");     // eth0 (ETHernet 0)

    QStringList interfaces = listOfInterfaces();

    Q_ASSERT(!interfaces.isEmpty());

    interfaces.sort();

    Q_FOREACH(const QString& prefix, prefixes)
    {
        Q_FOREACH(const QString& interface, interfaces)
        {
            if (interface.startsWith(prefix))
            {
                return interface;
            }
        }
    }
    return interfaces.first();
}

bool NetworkInformationReader::isNotableInterface(const QString& interface)
{
    return interface.startsWith("eth") || interface.startsWith("enp") ||
            interface.startsWith("wlan") || interface.startsWith("wlp");
}
