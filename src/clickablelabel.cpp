#include "clickablelabel.h"

// Copied and changed from: https://wiki.qt.io/Clickable_QLabel
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

/*
 * Copyright 2017, 2020 Carles Pina i Estany <carles@pina.cat>
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
