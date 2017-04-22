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
    m_maximumSpeedOut(0)
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
    if (m_informations.isEmpty())
    {
        return 0;
    }

    return m_informations.last().milliSecondsSinceEpoch - m_startedBytes.milliSecondsSinceEpoch;
}

NetworkInformationReader::NetworkBytesInOut InformationStorage::currentSpeed()
{
    if (m_informations.isEmpty())
    {
        NetworkInformationReader::NetworkBytesInOut networkBytesInOut;
        return networkBytesInOut;
    }
    return m_informations.last();
}

NetworkInformationReader::NetworkBytesInOut InformationStorage::calculateSpeed(const NetworkInformationReader::NetworkBytesInOut& before,
                                                         const NetworkInformationReader::NetworkBytesInOut& after)
{
    NetworkInformationReader::NetworkBytesInOut speed;

    float seconds_elapsed = (after.milliSecondsSinceEpoch-before.milliSecondsSinceEpoch)/1000.0;

    speed.in = (after.in-before.in) / seconds_elapsed;
    speed.out = (after.out-before.out) / seconds_elapsed;

    return speed;
}

void InformationStorage::addInformation(const NetworkInformationReader::NetworkBytesInOut& information)
{
    if (m_startedBytes.in == 0 || m_startedBytes.out == 0)
    {
        m_startedBytes = information;
        return;
    }

    // Instead of doing this we could do a circular list (should avoid reallocations)
    if (m_informations.size() == m_maximumInformation)
    {
        m_informations.removeFirst();
    }

    NetworkInformationReader::NetworkBytesInOut speed;

    // Information comes with total bytes, here we convert it into speed
    if (m_informations.isEmpty()) {
        // We use the initial transfer data
        speed = calculateSpeed(m_startedBytes, information);
    }
    else
    {
        speed = calculateSpeed(m_latestBytes, information);
    }
    speed.milliSecondsSinceEpoch = information.milliSecondsSinceEpoch;

    m_informations.append(speed);

    m_latestBytes = information;

    if (speed.in > m_maximumSpeedIn)
    {
        m_maximumSpeedIn = speed.in;
    }

    if (speed.out > m_maximumSpeedOut)
    {
        m_maximumSpeedOut = speed.out;
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
    if (m_informations.isEmpty())
    {
        return 0;
    }
    return m_latestBytes.in - m_startedBytes.in;
}

quint64 InformationStorage::transferredOut() const
{
    if (m_informations.isEmpty())
    {
        return 0;
    }

    return m_latestBytes.out - m_startedBytes.out;
}

QList<int> InformationStorage::lastValues(int numberOfValues, InOrOutType type)
{
    QList<int> values;

    int initial = qMax(0, m_informations.count()-numberOfValues);

    for (int i = initial; i < m_informations.count(); i++)
    {
        if (type == InType)
        {
            values.append(m_informations.at(i).in);
        }
        else if (type == OutType)
        {
            values.append(m_informations.at(i).out);
        }
    }

    return values;
}

