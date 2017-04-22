#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QTime>

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_networkInformation(0),
    m_informationStorage(new InformationStorage(this)),
    m_timer(0)
{
    ui->setupUi(this);

    // TODO: wlan0 should come from the command line arguments
    m_networkInformation = new NetworkInformationReader("wlan0", this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInformation()));
    m_timer->start(1000);

    ui->interface_and_time_label->setText(QString("%1: 0:00:00").arg(m_networkInformation->interfaceName()));
}

void MainWindow::updateInformation()
{
    NetworkInformationReader::NetworkBytesInOut information = m_networkInformation->readInformation();

    m_informationStorage->addInformation(information);

    quint64 maximumIn = m_informationStorage->maximumSpeedIn();
    quint64 maximumOut = m_informationStorage->maximumSpeedout();

    ui->interface_and_time_label->setText(QString("%1: %2").arg(m_networkInformation->interfaceName(),
                                                                formatMilliseconds(m_informationStorage->millisecondsSinceStart())));

    qDebug() << "MaximumIn:" << maximumIn;
    qDebug() << "MaximumOut:" << maximumOut;
}

QString MainWindow::padNumber(int number)
{
    return QString("%1").arg(number, 2, 10, QChar('0'));
}

QString MainWindow::formatMilliseconds(quint64 milliseconds)
{
    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;

    return QString("%1:%2:%3").arg(hours).arg(padNumber(minutes)).arg(padNumber(seconds));
}

MainWindow::~MainWindow()
{
    delete ui;
}
