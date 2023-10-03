#pragma once

#include <QWidget>

#include "informationstorage.h"

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

class Plot : public QWidget
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = nullptr);

    void setType(InformationStorage::InOrOutType inOrOutType);
    void setInformationStorage(InformationStorage* informationStorage);
    void update(InformationStorage* informationStorage);
    quint64 maximumValue();
    double maximumValueLog();


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);

signals:

public slots:

private:
    void paintScale(QPainter *painter);
    void paintBars(QPainter *painter);

    InformationStorage::InOrOutType m_type;
    InformationStorage* m_informationStorage;

    int m_paintedInitial;
};
