#include <QDebug>
#include <QDateTime>

#include "utils.h"

namespace Utils
{
quint64 currentMSecsSinceEpoch()
{
    return static_cast<quint64>(QDateTime::currentMSecsSinceEpoch());
}
}
