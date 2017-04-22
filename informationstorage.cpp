#include "informationstorage.h"
#include <QVector>
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

InformationStorage::InformationStorage(QObject *parent) :
    QObject(parent),
    m_maximumInformation(0),
    m_maximumSpeedIn(0),
    m_maximumSpeedOut(0),
    m_startsMillisecondsSinceEpoch(QDateTime::currentMSecsSinceEpoch()),
    m_startsInBytes(0),
    m_startsOutBytes(0)
{
    setCapacity(600);
}

void InformationStorage::setCapacity(int maximumInformation)
{
    if (maximumInformation > m_maximumInformation)
    {
        m_informations.reserve(maximumInformation);
    }
    m_maximumInformation = maximumInformation;
}

quint64 InformationStorage::millisecondsSinceStart() const
{
    if (m_informations.length() == 0)
    {
        return 0;
    }

    return m_informations.last().milliSecondsSinceEpoch - m_startsMillisecondsSinceEpoch;
}

InformationStorage::CurrentSpeed InformationStorage::calculateCurrentSpeed()
{
    if (m_informations.length() < 2)
    {
        return CurrentSpeed();
    }

    int lastIndex = m_informations.length()-1;
    int preLastIndex = m_informations.length()-2;

    qint64 elapsedMilliseconds = m_informations[lastIndex].milliSecondsSinceEpoch - m_informations[preLastIndex].milliSecondsSinceEpoch;
    quint64 bytesReceived = m_informations[lastIndex].in - m_informations[preLastIndex].in;
    quint64 bytesSent = m_informations[lastIndex].out - m_informations[preLastIndex].out;

    CurrentSpeed currentSpeed;
    currentSpeed.inSpeed = (bytesReceived / (elapsedMilliseconds/1000.0));
    currentSpeed.outSpeed = (bytesSent / (elapsedMilliseconds/1000.0));

    return currentSpeed;
}


void InformationStorage::addInformation(const NetworkInformationReader::NetworkBytesInOut& information)
{
    // Instead of doing this we could do a circular list (should avoid reallocations)
    if (m_informations.size() == m_maximumInformation)
    {
        m_informations.removeFirst();
    }

    m_informations.append(information);

    CurrentSpeed currentSpeed = calculateCurrentSpeed();

    if (currentSpeed.inSpeed > m_maximumSpeedIn)
    {
        m_maximumSpeedIn = currentSpeed.inSpeed;
    }

    if (currentSpeed.outSpeed > m_maximumSpeedOut)
    {
        m_maximumSpeedOut = currentSpeed.outSpeed;
    }

    if (m_startsInBytes == 0)
    {
        m_startsInBytes = information.in;
    }

    if (m_startsOutBytes == 0)
    {
        m_startsOutBytes = information.out;
    }
}

quint64 InformationStorage::maximumSpeedIn() const
{
    return m_maximumSpeedIn;
}

quint64 InformationStorage::maximumSpeedOut() const
{
    return m_maximumSpeedOut;
}

quint64 InformationStorage::transferredIn() const
{
    return m_informations.last().in - m_startsInBytes;
}

quint64 InformationStorage::transferredOut() const
{
    return m_informations.last().out - m_startsOutBytes;
}

