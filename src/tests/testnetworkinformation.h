#include <QTest>

#include "../networkinformationreader.h"

class TestNetworkInformation : public QObject
{
Q_OBJECT

public:
    TestNetworkInformation();

private Q_SLOTS:
    void testIsValid();
    void testReadInformation();

private:
    QScopedPointer<NetworkInformationReader> m_networkInformationReader;
};
