#include <QTest>

/*
 * Copyright 2017 Carles Pina i Estany <carles@pina.cat>
 * This file is part of qnetload.
 *
 * Foobar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qnetload.  If not, see <http://www.gnu.org/licenses/>.
 */

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
