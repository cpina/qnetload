#include "networkinformationreader.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

/*
 * Copyright 2017 Carles Pina i Estany <carles@pina.cat>
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

NetworkInformationReader::NetworkInformationReader(const QString& interfaceName, QObject* parent)
    : QObject(parent)
    , m_procNetDev("/proc/net/dev")
{
    m_interfaceName = interfaceName;
}


NetworkInformationReader::NetworkInformationReader(const QString& interfaceName,
                                                   const QString& procNetDevFile,
                                                   QObject* parent)
    : QObject(parent)
    , m_procNetDev(procNetDevFile)
{
    m_interfaceName = interfaceName;
}

QString NetworkInformationReader::procNetDev() const
{
    return m_procNetDev;
}

bool NetworkInformationReader::isValid() const
{
    NetworkBytesInOut bytesInOut = readProcNetDevInterface(m_interfaceName);

    return bytesInOut.valid;
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
            information.milliSecondsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
            information.valid = true;

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
/*    if (match.hasMatch())
    {
        NetworkInformationReader::NetworkBytesInOut information;
        information.in = match.captured(2).toULong();
        information.out = match.captured(3).toULong();
        information.milliSecondsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
        information.valid = true;

        return information;
    }
    else
    {
        return NetworkInformationReader::NetworkBytesInOut();
    }*/
}

NetworkInformationReader::NetworkBytesInOut NetworkInformationReader::readInformation() const
{
    NetworkInformationReader::NetworkBytesInOut information = readProcNetDevInterface(m_interfaceName);
    return information;
}
