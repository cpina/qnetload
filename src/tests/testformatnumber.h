#include <QTest>

class TestFormatNumber : public QObject
{
Q_OBJECT

public:
    TestFormatNumber();

private Q_SLOTS:
    void testFormatElapsedTime_data();
    void testFormatElapsedTime();

    void testFormatSpeed_data();
    void testFormatSpeed();

    void testFormatTransfer_data();
    void testFormatTransfer();
};
