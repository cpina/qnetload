#ifndef INFORMATIONSTORAGE_H
#define INFORMATIONSTORAGE_H

#include <QObject>
#include <QVector>
#include "networkinformationreader.h"

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

class InformationStorage : public QObject
{
    Q_OBJECT

public:
    enum InOrOutType
    {
        InType,
        OutType,
        UndefinedType
    };

    explicit InformationStorage(QObject *parent = 0);

    void addInformation(const NetworkInformationReader::NetworkBytesInOut& information);
    quint64 maximumSpeedIn() const;
    quint64 maximumSpeedOut() const;
    quint64 transferredIn() const;
    quint64 transferredOut() const;
    quint64 millisecondsSinceStart() const;

    quint64 speed(int position, InOrOutType inOrOut) const;

    void setCapacity(int maximumInformation);

    QList<int> lastValues(int numberOfValues, InOrOutType type);

    NetworkInformationReader::NetworkBytesInOut currentSpeed();

    QVector<NetworkInformationReader::NetworkBytesInOut> informations() const;

    quint64 accumulatedTransfer(int position, InformationStorage::InOrOutType inOrOut) const;

    quint64 secondsAgo(int position);

    bool isPaused() const;

public slots:
    void initialize();

    void pause(const NetworkInformationReader::NetworkBytesInOut& networkBytesInOut);
    void unpause(NetworkInformationReader::NetworkBytesInOut& networkBytesInOut);

private:
    NetworkInformationReader::NetworkBytesInOut calculateSpeed(const NetworkInformationReader::NetworkBytesInOut& before,
                                                               const NetworkInformationReader::NetworkBytesInOut& after);

    int m_maximumInformation;
    QVector<NetworkInformationReader::NetworkBytesInOut> m_informations;

    quint64 m_maximumSpeedIn;
    quint64 m_maximumSpeedOut;

    NetworkInformationReader::NetworkBytesInOut m_startedBytes;
    NetworkInformationReader::NetworkBytesInOut m_latestBytes;

    quint64 m_pausedInBytes;
    quint64 m_pausedOutBytes;

    bool m_isPaused;
    NetworkInformationReader::NetworkBytesInOut m_pauseStartsNetworkBytesInOut;


};

#endif // INFORMATIONSTORAGE_H
