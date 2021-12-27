#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include "networkinformationreader.h"
#include "informationstorage.h"

class QToolButton;

/*
 * Copyright 2017, 2019, 2021 Carles Pina i Estany <carles@pina.cat>
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
    enum TimeType
    {
        millisecondsSinceStart = 0,
        timeStarted = 1
    };
    Q_ENUM(TimeType);

    explicit MainWindow(const QString& interfaceName, QWidget *parent = 0);
    ~MainWindow();

protected:
    void wheelEvent(QWheelEvent* event);

private Q_SLOTS:
    void updateInformation();
    void selectNextInterface();
    void toggleTime();

    void showContextualMenu(const QPoint& position);
    void interfaceNameChanged();

    void resetCountDown();
    void togglePause();

    void copyIp();


private:
    void reset();
    int readCurrentFontSize() const;
    QString chooseInterfaceName() const;
    void setAllLabels(const QString& interfaceName, quint64 millisecondsSinceStart,
                      const QString& timeStarted,
                      quint64 currentSpeedIn, quint64 maximumSpeedIn, quint64 transferredIn,
                      quint64 currentSpeedOut, quint64 maximumSpeedOut, quint64 transferredOut,
                      quint64 transferredTotal);
    void setTooltips();
    void setFontSize(int fontSize);

    void updateButtonIcon(QToolButton* toolButton, const QPixmap& pixmap);

    Ui::MainWindow *ui;
    NetworkInformationReader* m_networkInformation;
    InformationStorage* m_informationStorage;
    QTimer* m_timer;

    QPixmap m_resetPixmap;
    QPixmap m_pausePixmap;

    bool m_resetWaitingForConfirmation;
    int m_resetCountDown;
    TimeType m_timeType;
};

#endif // MAINWINDOW_H
