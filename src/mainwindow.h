#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkinformationreader.h"
#include "informationstorage.h"

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& interfaceName, QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:
    void updateInformation();

private:
    QString chooseInterfaceName() const;
    void setAllLabels(const QString& interfaceName, quint64 millisecondsSinceStart,
                      quint64 currentSpeedIn, quint64 maximumSpeedIn, quint64 transferredIn,
                      quint64 currentSpeedOut, quint64 maximumSpeedOut, quint64 transferredOut);
    void setTooltips();

    Ui::MainWindow *ui;
    NetworkInformationReader* m_networkInformation;
    InformationStorage* m_informationStorage;
    QTimer* m_timer;
};

#endif // MAINWINDOW_H
