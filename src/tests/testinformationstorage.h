#include <QTest>

#include "../informationstorage.h"

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

class TestInformationStorage : public QObject
{
Q_OBJECT

public:
    TestInformationStorage();

private Q_SLOTS:
    void init();

    void testCurrentSpeed();

    void testMaximumSpeedIn();
    void testMaximumSpeedOut();

    void testTransferredIn();
    void testTransferredOut();

    void testMillisecondsSinceStart();

private:
    void addData(quint64 in, quint64 out, qint64 millisecondsSinceEpoch);

    QScopedPointer<InformationStorage> m_informationStorage;
};
