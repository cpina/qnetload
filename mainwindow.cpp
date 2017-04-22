#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formatnumber.h"

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
    ui->in_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(0),
                                                          FormatNumber::formatSpeed(0),
                                                          FormatNumber::formatTransfer(0)));

    ui->out_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(0),
                                                           FormatNumber::formatSpeed(0),
                                                           FormatNumber::formatTransfer(0)));

    updateInformation();
}

void MainWindow::updateInformation()
{
    NetworkInformationReader::NetworkBytesInOut information = m_networkInformation->readInformation();

    m_informationStorage->addInformation(information);

    quint64 maximumIn = m_informationStorage->maximumSpeedIn();
    quint64 maximumOut = m_informationStorage->maximumSpeedOut();

    quint64 transferredIn = m_informationStorage->transferredIn();
    quint64 transferredOut = m_informationStorage->transferredOut();

    InformationStorage::CurrentSpeed currentSpeed = m_informationStorage->calculateCurrentSpeed();
    quint64 currentSpeedIn = currentSpeed.inSpeed;
    quint64 currentSpeedOut = currentSpeed.outSpeed;


    ui->interface_and_time_label->setText(QString("%1: %2").arg(m_networkInformation->interfaceName(),
                                                                FormatNumber::formatElapsedTime(m_informationStorage->millisecondsSinceStart())));

    ui->in_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(currentSpeedIn),
                                                          FormatNumber::formatSpeed(maximumIn),
                                                          FormatNumber::formatTransfer(transferredIn)));

    ui->out_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(currentSpeedOut),
                                                           FormatNumber::formatSpeed(maximumOut),
                                                           FormatNumber::formatTransfer(transferredOut)));

    qDebug() << "MaximumIn:" << maximumIn;
    qDebug() << "MaximumOut:" << maximumOut;
}

MainWindow::~MainWindow()
{
    delete ui;
}
