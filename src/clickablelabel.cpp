#include "clickablelabel.h"

// Copied and changed from: https://wiki.qt.io/Clickable_QLabel

#include <QDebug>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
