#include <QTest>

class TestNetworkInformation : public QObject
{
Q_OBJECT

public:
TestNetworkInformation() {};

private Q_SLOTS:
void testIsValid();

};
