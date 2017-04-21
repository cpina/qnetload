#include "networkinformation.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

/*
 * Copyright 2017 Carles Pina i Estany <carles@pina.cat>
 * This file is part of qnetload.
 *
 * Foobar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qnetload.  If not, see <http://www.gnu.org/licenses/>.
 */

NetworkInformation::NetworkInformation(const QString& interfaceName, QObject* parent)
    :
    QObject(parent)
{
    m_interfaceName = interfaceName;
}

bool NetworkInformation::verifyInterface() const
{
    // TODO: if it doesn't exist return false
    return true;
}

NetworkInformation::NetworkBytesInOut NetworkInformation::readProcNetDevInterface(const QString& interface)
{
    QFile file("/proc/net/dev");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return NetworkInformation::NetworkBytesInOut();
    }

    QString line;
    do
    {
        line = file.readLine();

        NetworkInformation::NetworkBytesInOut information = readInformationFromLine(line, interface);

        if (information.valid)
        {
            return information;
        }

    } while(!line.isEmpty());

    return NetworkInformation::NetworkBytesInOut();
}

NetworkInformation::NetworkBytesInOut NetworkInformation::readInformationFromLine(const QString& line, const QString& interface)
{
    QRegularExpression lineInformationRe(QString(" *%1: * (\\d+) +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +(\\d+).*").arg(interface));

    QRegularExpressionMatch match = lineInformationRe.match(line);

    if (match.hasMatch())
    {
        NetworkInformation::NetworkBytesInOut information;
        information.in = match.captured(1).toULong();
        information.out = match.captured(2).toULong();
        information.milliSecondsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
        information.valid = true;

        return information;
    }
    else
    {
        return NetworkInformation::NetworkBytesInOut();
    }
}

NetworkInformation::NetworkBytesInOut NetworkInformation::readInformation() const
{
    NetworkInformation::NetworkBytesInOut information = readProcNetDevInterface(m_interfaceName);

    qDebug() << "In :" << information.in;
    qDebug() << "Out:" << information.out;
    qDebug() << "milliSecondsSinceEpoch:" << information.milliSecondsSinceEpoch;

    return information;
}
