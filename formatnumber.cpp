#include "formatnumber.h"

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

QString FormatNumber::formatBytes(quint64 bytes, const QString& unitsPostfix)
{
    float value = bytes;

    int i;
    for (i = 0; value > 1024.0 && i < prefixes().count(); i++)
    {
        value /= 1024.0;
    }
    return QString("%1 %2%3").arg(QString::number(value, 'f', 1)).arg(prefixes()[i], unitsPostfix);
}

QString FormatNumber::formatSpeed(quint64 bytesSecond)
{
    return formatBytes(bytesSecond, "B/s");
}

QString FormatNumber::formatTransfer(quint64 bytes)
{
    return formatBytes(bytes, "B");
}

QString FormatNumber::formatElapsedTime(quint64 milliseconds)
{
    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;

    return QString("%1:%2:%3").arg(hours).arg(padNumber(minutes)).arg(padNumber(seconds));
}

QString FormatNumber::padNumber(int number)
{
    return QString("%1").arg(number, 2, 10, QChar('0'));
}


QStringList FormatNumber::prefixes()
{
    static QStringList prefixList = (QStringList() << "" << "k" << "M" << "G" << "T" << "P" << "E");
    return prefixList;
}
