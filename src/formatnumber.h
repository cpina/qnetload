#ifndef FORMATBYTES_H
#define FORMATBYTES_H

#include <QString>
#include <QStringList>

/*
 * Copyright 2017, 2019 Carles Pina i Estany <carles@pina.cat>
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

class FormatNumber
{
public:
    static QString formatSpeed(quint64 bytesSecond);
    static QString formatTransfer(quint64 bytes);

    // Formats to hh:mm:ss
    static QString formatElapsedTime(quint64 milliseconds);

    // formats to M min S sec
    static QString formatSeconds(quint64 totalSeconds);

private:
    static QString formatBytes(quint64 bytes, const QString& unitsPostfix);

    static QString padNumber(int number);
    static QStringList prefixes();
};

#endif // FORMATBYTES_H
