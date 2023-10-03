#pragma once

#include <QObject>
#include <QString>

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

class NetworkInformationReader : public QObject
{
    Q_OBJECT

public:
    struct NetworkBytesInOut {
        quint64 in;
        quint64 out;
        quint64 milliSecondsSinceEpoch;

        NetworkBytesInOut() :
            in(0),
            out(0),
            milliSecondsSinceEpoch(0)
        {}

        NetworkBytesInOut(const NetworkBytesInOut& networkBytesInOut_) :
            in(networkBytesInOut_.in),
            out(networkBytesInOut_.out),
            milliSecondsSinceEpoch(networkBytesInOut_.milliSecondsSinceEpoch)
        {}
    };

    NetworkInformationReader(QObject* parent=0);
    NetworkInformationReader(const QString &interfaceName, const QString& procNetDevPath, QObject* parent=0);

    void setInterfaceName(const QString& interfaceName);
    QString interfaceName() const;
    NetworkBytesInOut readInformation() const;

    void setProcNetDev(const QString& path);
    QString procNetDev() const;

    NetworkBytesInOut readProcNetDevInterface(const QString& interface) const;

    QStringList listOfInterfaces() const;

    QString chooseInterfaceFromProcNetDevInterface() const;

    QString ip() const;

    static bool isNotableInterface(const QString& interface);

signals:
    void interfaceNameChanged();

private:
    static QRegularExpressionMatch parseLine(const QString &line);
    QString m_interfaceName;
    QString m_procNetDev;
};
