#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#include <QObject>
#include <QString>

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

class NetworkInformationReader : public QObject
{
public:
    struct NetworkBytesInOut {
        quint64 in;
        quint64 out;
        qint64 milliSecondsSinceEpoch;
        bool valid;

        NetworkBytesInOut() : in(0), out(0), milliSecondsSinceEpoch(0), valid(false) {}
    };

    NetworkInformationReader(const QString& interfaceName, QObject* parent=0);
    bool verifyInterface() const;
    NetworkBytesInOut readInformation() const;


private:
    static NetworkBytesInOut readInformationFromLine(const QString &line, const QString &interface);
    static NetworkBytesInOut readProcNetDevInterface(const QString& interface);
    QString m_interfaceName;
};

#endif // NETWORKINFORMATION_H
