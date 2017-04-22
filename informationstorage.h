#ifndef INFORMATIONSTORAGE_H
#define INFORMATIONSTORAGE_H

#include <QObject>
#include <QVector>
#include "networkinformationreader.h"

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

class InformationStorage : public QObject
{
public:
    struct CurrentSpeed
    {
        quint64 inSpeed;
        quint64 outSpeed;

        CurrentSpeed() : inSpeed(0), outSpeed(0) {}
    };

    explicit InformationStorage(QObject *parent = 0);

    void addInformation(const NetworkInformationReader::NetworkBytesInOut& information);
    quint64 maximumSpeedIn() const;
    quint64 maximumSpeedOut() const;
    quint64 transferredIn() const;
    quint64 transferredOut() const;
    quint64 millisecondsSinceStart() const;

    void setCapacity(int maximumInformation);

    CurrentSpeed calculateCurrentSpeed();

private:
    int m_maximumInformation;
    QVector<NetworkInformationReader::NetworkBytesInOut> m_informations;

    quint64 m_maximumSpeedIn;
    quint64 m_maximumSpeedOut;
    quint64 m_startsMillisecondsSinceEpoch;
    quint64 m_startsInBytes;
    quint64 m_startsOutBytes;
};

#endif // INFORMATIONSTORAGE_H
