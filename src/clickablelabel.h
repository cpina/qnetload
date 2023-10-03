#pragma once
// Copied and changed from https://wiki.qt.io/Clickable_QLabel

#include <QLabel>
#include <QWidget>

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


class ClickableLabel : public QLabel
{
	Q_OBJECT

	public:
		explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
		~ClickableLabel();

	Q_SIGNALS:
        void leftClicked();

	protected:
        void mousePressEvent(QMouseEvent* event) override;

        void leaveEvent(QEvent* event) override;
        void enterEvent(QEnterEvent* event) override;

        virtual void paintEvent(QPaintEvent* event) override;

    private:
        bool m_inside;
};
