#include "plot.h"

#include "formatnumber.h"
#include "networkinformationreader.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QToolTip>

#include <math.h>

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

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    m_type(InformationStorage::UndefinedType),
    m_paintedInitial(0)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
}

int Plot::maximumValue()
{
    return std::max(m_informationStorage->maximumSpeedIn(), m_informationStorage->maximumSpeedOut());
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
    QVector<NetworkInformationReader::NetworkBytesInOut> informations = m_informationStorage->informations();

    float maximumValue = maximumValueLog();

    int initial = qMax(0, informations.count()-width());

    m_paintedInitial = initial;

    int x;
    for(int i=initial; i < informations.count(); i++)
    {
        float y;
        quint64 speed = m_informationStorage->speed(i, m_type);
        if (speed != 0)
        {
            y = (height() * log(speed)) / maximumValue;

        }
        else
        {
            y = 0;
        }

        y = height() - y;
        x = i - initial;

        painter->drawLine(QPoint(x, height()),
                          QPoint(x, y));
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

void Plot::mousePressEvent(QMouseEvent* event)
{
    int valuePosition = m_paintedInitial + event->x();

    if (valuePosition > m_informationStorage->informations().count())
    {
        return;
    }

    QString timeAgo = FormatNumber::formatSeconds(m_informationStorage->secondsAgo(valuePosition));

    quint64 accumulatedTransferIn = m_informationStorage->accumulatedTransfer(valuePosition, InformationStorage::InType);
    quint64 accumulatedTransferOut = m_informationStorage->accumulatedTransfer(valuePosition, InformationStorage::OutType);
    quint64 accumulatedTransfer = accumulatedTransferIn + accumulatedTransferOut;

    QString information = QString("Last %1\nIn: %2 Out: %3\nTotal: %4 transferred").arg(timeAgo)
                                                                                 .arg(FormatNumber::formatTransfer(accumulatedTransferIn))
                                                                                 .arg(FormatNumber::formatTransfer(accumulatedTransferOut))
                                                                                 .arg(FormatNumber::formatTransfer(accumulatedTransfer));

    QToolTip::showText(event->globalPos(), information);
}

void Plot::setType(InformationStorage::InOrOutType inOrOutType)
{
    m_type = inOrOutType;
}

void Plot::setInformationStorage(InformationStorage* informationStorage)
{
    m_informationStorage = informationStorage;
}
