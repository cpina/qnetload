#include "informationstorage.h"
#include <QVector>
#include <QDateTime>
#include <QDebug>

/*
 * Copyright 2017, 2020 Carles Pina i Estany <carles@pina.cat>
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

InformationStorage::InformationStorage(QObject *parent) :
    QObject(parent)
{
    initialize();
}

void InformationStorage::initialize()
{
    m_maximumInformation = 0;
    m_maximumSpeedIn = 0;
    m_maximumSpeedOut = 0;

    m_informations.clear();
    m_startedBytes = NetworkInformationReader::NetworkBytesInOut();
    m_latestBytes = NetworkInformationReader::NetworkBytesInOut();

    setCapacity(4096);
    m_isPaused = false;

    m_pausedInBytes = 0;
    m_pausedOutBytes = 0;
}

void InformationStorage::pause()
{
    m_isPaused = true;
}

void InformationStorage::unpause()
{
    m_isPaused = false;
}

bool InformationStorage::isPaused() const
{
    return m_isPaused;
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
    return QDateTime::currentMSecsSinceEpoch() - m_startedBytes.milliSecondsSinceEpoch;
}

quint64 InformationStorage::accumulatedTransfer(int position, InformationStorage::InOrOutType inOrOut) const
{
    quint64 dataAccumulated = 0;

    for (int i = position; i < m_informations.count(); i++)
    {
        dataAccumulated += speed(i, inOrOut);
    }
    return dataAccumulated;
}

quint64 InformationStorage::secondsAgo(int position)
{
    quint64 milliSecondsSinceEpoch = m_informations[position].milliSecondsSinceEpoch;
    return (QDateTime::currentMSecsSinceEpoch() - milliSecondsSinceEpoch) / 1000;
}

bool InformationStorage::wasPaused(int position) const
{
    return m_informations[position].paused;
}

quint64 InformationStorage::speed(int position, InformationStorage::InOrOutType inOrOut) const
{
    switch(inOrOut)
    {
        case InOrOutType::InType:
            return m_informations[position].in;
        case InOrOutType::OutType:
            return m_informations[position].out;
    }
    __builtin_unreachable();
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

InformationStorage::NetworkBytesInOutPaused InformationStorage::calculateSpeed(const NetworkInformationReader::NetworkBytesInOut& before,
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
    if (m_startedBytes.milliSecondsSinceEpoch == 0)
    {
        // It hasn't been initialized yet
        m_startedBytes = information;
        return;
    }

    // Instead of doing this we could do a circular list (should avoid reallocations)
    if (m_informations.size() == m_maximumInformation)
    {
        m_informations.removeFirst();
    }

    InformationStorage::NetworkBytesInOutPaused speed;

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

    speed.paused = isPaused();

    m_informations.append(speed);

    if (!isPaused())
    {
        if (speed.in > m_maximumSpeedIn)
        {
            m_maximumSpeedIn = speed.in;
        }

        if (speed.out > m_maximumSpeedOut)
        {
            m_maximumSpeedOut = speed.out;
        }
    }
    else
    {
        m_pausedInBytes += information.in - m_latestBytes.in;
        m_pausedOutBytes += information.out - m_latestBytes.out;
    }
    m_latestBytes = information;
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
    return m_latestBytes.in - m_startedBytes.in - m_pausedInBytes;
}

quint64 InformationStorage::transferredOut() const
{
    if (m_informations.isEmpty())
    {
        return 0;
    }

    return m_latestBytes.out - m_startedBytes.out - m_pausedOutBytes;
}

QVector<InformationStorage::NetworkBytesInOutPaused> InformationStorage::informations() const
{
    return m_informations;
}
