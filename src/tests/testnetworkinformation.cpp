#include "testnetworkinformation.h"

#include <QDebug>

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

TestNetworkInformation::TestNetworkInformation()
{

}

void TestNetworkInformation::testReadInformation()
{
    m_networkInformationReader.reset(new NetworkInformationReader("eth1", "proc_net_dev_files/example01.txt"));

    quint64 before = QDateTime::currentMSecsSinceEpoch();
    NetworkInformationReader::NetworkBytesInOut information = m_networkInformationReader->readInformation();


    QCOMPARE(information.in, quint64(11546469));
    QCOMPARE(information.out, quint64(1058947));
    QVERIFY(information.milliSecondsSinceEpoch >= before);
    QVERIFY(information.milliSecondsSinceEpoch <= QDateTime::currentMSecsSinceEpoch());
}

void TestNetworkInformation::testIsValid()
{
    m_networkInformationReader.reset(new NetworkInformationReader("non_existing", "proc_net_dev_files/example01.txt"));

    QVERIFY(!m_networkInformationReader->isValid());

    m_networkInformationReader.reset(new NetworkInformationReader("eth1", "proc_net_dev_files/example01.txt"));
    QVERIFY(m_networkInformationReader->isValid());
}

QTEST_MAIN(TestNetworkInformation)
