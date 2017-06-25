#include "testinformationstorage.h"

#include <QDebug>

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

TestInformationStorage::TestInformationStorage()
{
}

void TestInformationStorage::init()
{
    m_informationStorage.reset(new InformationStorage);
}

void TestInformationStorage::testLastValues()
{
    // It also checks indirectly calculating the speed
    QCOMPARE(m_informationStorage->lastValues(10, InformationStorage::InType), QList<int>());

    addData(10000, 400, 1000);
    addData(20000, 800, 1500);

    QList<int> actual = m_informationStorage->lastValues(10, InformationStorage::InType);

    QCOMPARE(actual, QList<int>() << 20000);

    addData(30000, 1000, 2000);
    addData(40000, 1200, 2500);
    addData(50000, 1400, 3000);
    addData(60000, 1600, 4500);
    addData(70000, 1800, 5000);
    addData(80000, 2000, 5500);
    addData(95000, 2200, 6000);    // Speed in: (95000-80000) / 0.5 = 30000
    addData(100000, 2400, 6500);    // Speed in: (100000-95000) / 0.5 = 10000

    actual = m_informationStorage->lastValues(2, InformationStorage::InType);
    QCOMPARE(actual, QList<int>() << 30000 << 10000);

    actual = m_informationStorage->lastValues(2, InformationStorage::OutType);
    QCOMPARE(actual, QList<int>() << 400 << 400);
}

void TestInformationStorage::testCurrentSpeed()
{
    addData(10000, 400, 1000);
    addData(20000, 400, 2000);

    NetworkInformationReader::NetworkBytesInOut currentSpeed = m_informationStorage->currentSpeed();

    QCOMPARE(currentSpeed.in, quint64(10000));
    QCOMPARE(currentSpeed.out, quint64(0));
}

void TestInformationStorage::testMaximumSpeedIn()
{
    addData(10000, 400, 1000);
    addData(40000, 400, 2000);
    addData(50000, 10000, 3000);

    quint64 maximumSpeedIn = m_informationStorage->maximumSpeedIn();

    QCOMPARE(maximumSpeedIn, quint64(30000));
}

void TestInformationStorage::testMaximumSpeedOut()
{
    addData(10000, 400, 1000);
    addData(40000, 400, 2000);
    addData(50000, 10000, 3000);

    quint64 maximumSpeedIn = m_informationStorage->maximumSpeedOut();

    QCOMPARE(maximumSpeedIn, quint64(9600));
}

void TestInformationStorage::testTransferredIn()
{
    m_informationStorage->setCapacity(1);
    addData(10000, 400, 1000);
    addData(50000, 500, 1001);

    QCOMPARE(m_informationStorage->transferredIn(), quint64(40000));
}

void TestInformationStorage::testTransferredOut()
{
    m_informationStorage->setCapacity(1);
    addData(10000, 400, 1000);
    addData(50000, 500, 1001);

    QCOMPARE(m_informationStorage->transferredOut(), quint64(100));
}

void TestInformationStorage::testMillisecondsSinceStart()
{
    m_informationStorage->setCapacity(1);

    quint64 millisecondsTimestamp = QDateTime::currentMSecsSinceEpoch();

    addData(1000, 400, millisecondsTimestamp);
    addData(1000, 400, millisecondsTimestamp);
    addData(1000, 400, millisecondsTimestamp);

    quint64 millisecondsSinceStart = m_informationStorage->millisecondsSinceStart();
    QVERIFY(millisecondsSinceStart <= QDateTime::currentMSecsSinceEpoch() - millisecondsTimestamp);
}

void TestInformationStorage::addData(quint64 in, quint64 out, qint64 millisecondsSinceEpoch)
{
    NetworkInformationReader::NetworkBytesInOut data;
    data.in = in;
    data.out = out;
    data.milliSecondsSinceEpoch = millisecondsSinceEpoch;

    m_informationStorage->addInformation(data);
}

QTEST_MAIN(TestInformationStorage)
