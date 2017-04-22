#include "plot.h"

#include <QDebug>
#include <QPainter>

#include <math.h>

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

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    m_type(InformationStorage::UndefinedType)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
}

int Plot::maximumValue()
{
    int maximum = 0;
    switch (m_type)
    {
        case InformationStorage::InType:
            maximum = m_informationStorage->maximumSpeedIn();
            break;
        case InformationStorage::OutType:
            maximum = m_informationStorage->maximumSpeedOut();
            break;
        default:
            Q_ASSERT(false);
    }
    return maximum;
}

float Plot::maximumValueLog()
{
    return log(maximumValue());
}

void Plot::paintScale(QPainter* painter)
{
    float logMaximum = maximumValueLog();

    float space_between_horizontal_lines = height() / logMaximum;

    int line_number = 0;
    for (float y = height(); y > space_between_horizontal_lines/2; y-= space_between_horizontal_lines, line_number++)
    {
        if (space_between_horizontal_lines < 15 && line_number % 2 == 1)
        {
            continue;
        }
        painter->drawLine(QPoint(0, y),
                         QPoint(width(), y));
    }
}

void Plot::paintBars(QPainter *painter)
{
    QList<int> values = m_informationStorage->lastValues(width(), InformationStorage::InType);

    float maximumValue = maximumValueLog();

    for (int i = 0; i < values.count(); i++)
    {
        float y;
        if (values[i] != 0)
        {
            y = (height() * log(values[i])) / maximumValue;

        }
        else
        {
            y = 0;
        }

        y = height() - y;


        painter->drawLine(QPoint(i, height()),
                          QPoint(i, y));
    }
}

void Plot::paintEvent(QPaintEvent *event)
{
    int maxValue = maximumValue();

    if (maxValue == 0)
    {
        return;
    }
    QPainter painter(this);
    paintScale(&painter);
    paintBars(&painter);
}

void Plot::setType(InformationStorage::InOrOutType inOrOutType)
{
    m_type = inOrOutType;
}

void Plot::setInformationStorage(InformationStorage* informationStorage)
{
    m_informationStorage = informationStorage;
}
