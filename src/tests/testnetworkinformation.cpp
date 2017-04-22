#include "testnetworkinformation.h"

#include <QDebug>

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
