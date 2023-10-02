#include "testformatnumber.h"
#include "../formatnumber.h"

#include <QDebug>

/*
 * Copyright 2017, 2023 Carles Pina i Estany <carles@pina.cat>
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

TestFormatNumber::TestFormatNumber()
{

}

void TestFormatNumber::testFormatElapsedTime_data()
{
    QTest::addColumn<int>("milliseconds");
    QTest::addColumn<QString>("humanReadable");

    QTest::newRow("num1") << 100*1000 << QString("0:01:40");
    QTest::newRow("num2") << 25*3600*1000 << QString("25:00:00");
    QTest::newRow("num3") << (25*3600+100)*1000 << QString("25:01:40");
}

void TestFormatNumber::testFormatElapsedTime()
{
    QFETCH(int, milliseconds);
    QFETCH(QString, humanReadable);

    QCOMPARE(FormatNumber::formatElapsedTime(milliseconds), humanReadable);
}

void TestFormatNumber::testFormatSpeed_data()
{
    QTest::addColumn<quint64>("bytesSecond");
    QTest::addColumn<QString>("humanReadable");

    QTest::newRow("num1") << quint64(10) << QString("10 B/s");
    QTest::newRow("num2") << quint64(1023) << QString("1023 B/s");
    QTest::newRow("num3") << quint64(1025) << QString("1.00 KB/s");
    QTest::newRow("num4") << quint64(1024*1024*1024) << QString("1024.00 MB/s");
    QTest::newRow("num5") << quint64(2)*1024*1024*1024 << QString("2.00 GB/s");
}

void TestFormatNumber::testFormatSpeed()
{
    QFETCH(quint64, bytesSecond);
    QFETCH(QString, humanReadable);

    QCOMPARE(FormatNumber::formatSpeed(bytesSecond), humanReadable);
}


void TestFormatNumber::testFormatTransfer_data()
{
    QTest::addColumn<quint64>("bytes");
    QTest::addColumn<QString>("humanReadable");

    QTest::newRow("num1") << quint64(10) << QString("10 B");
    QTest::newRow("num2") << quint64(1023) << QString("1023 B");
    QTest::newRow("num3") << quint64(1025) << QString("1.00 KB");
    QTest::newRow("num4") << quint64(1024*1024*1024) << QString("1024.00 MB");
    QTest::newRow("num5") << quint64(2)*1024*1024*1024 << QString("2.00 GB");
}

void TestFormatNumber::testFormatTransfer()
{
    QFETCH(quint64, bytes);
    QFETCH(QString, humanReadable);

    QCOMPARE(FormatNumber::formatTransfer(bytes), humanReadable);
}

QTEST_MAIN(TestFormatNumber)
