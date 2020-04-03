#include "clickablelabel.h"

// Copied and changed from: https://wiki.qt.io/Clickable_QLabel
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
    , m_inside(false)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        emit leftClicked();
    }
}

void ClickableLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::black);
    if (m_inside)
    {
        painter.drawRect(0, 0, width()-1, height()-1);
    }
}

void ClickableLabel::enterEvent(QEvent* event)
{
    m_inside = true;
    update();
}

void ClickableLabel::leaveEvent(QEvent* event)
{
    m_inside = false;
    update();
}
