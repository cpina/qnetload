#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formatnumber.h"

#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QMessageBox>

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

MainWindow::MainWindow(const QString& interfaceName, const QString& helpText, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_networkInformation(0),
    m_informationStorage(new InformationStorage(this)),
    m_timer(0)
{
    ui->setupUi(this);

    m_networkInformation = new NetworkInformationReader(interfaceName, this);

    QStringList listOfInterfaces = m_networkInformation->listOfInterfaces();
    if (interfaceName.isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("qnetload"),
                              QObject::tr("Please pass the interface name as first argument when executing qnetload.\n\n"
                                          "It could be: %2").arg(listOfInterfaces.join(", ")));
        qWarning() << helpText;
        exit(3);
    }

    if (!listOfInterfaces.contains(interfaceName))
    {
        QString message1 = tr("%1: %2 interface not found in %3").arg(QApplication::applicationName())
                                                                     .arg(interfaceName)
                                                                     .arg(m_networkInformation->procNetDev());
        qWarning() << message1;

        QString message2 = tr("Instead of %1 use one of those: %2").arg(interfaceName).arg(listOfInterfaces.join(", "));
        qWarning() << message2;

        QMessageBox::critical(this, tr("qnetload"), message1 + "\n" + message2);

        exit(2);
    }

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInformation()));
    m_timer->start(1000);

    ui->interface_and_time_label->setText(QString("%1: 0:00:00").arg(m_networkInformation->interfaceName()));
    ui->in_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(0),
                                                          FormatNumber::formatSpeed(0),
                                                          FormatNumber::formatTransfer(0)));

    ui->out_label->setText(QString("out: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(0),
                                                            FormatNumber::formatSpeed(0),
                                                            FormatNumber::formatTransfer(0)));

    ui->in_graph->setType(InformationStorage::InType);
    ui->in_graph->setInformationStorage(m_informationStorage);

    ui->out_graph->setType(InformationStorage::OutType);
    ui->out_graph->setInformationStorage(m_informationStorage);

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

    NetworkInformationReader::NetworkBytesInOut currentSpeed = m_informationStorage->currentSpeed();
    quint64 currentSpeedIn = currentSpeed.in;
    quint64 currentSpeedOut = currentSpeed.out;


    ui->interface_and_time_label->setText(QString("%1: %2").arg(m_networkInformation->interfaceName(),
                                                                FormatNumber::formatElapsedTime(m_informationStorage->millisecondsSinceStart())));

    ui->in_label->setText(QString("in: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(currentSpeedIn),
                                                          FormatNumber::formatSpeed(maximumIn),
                                                          FormatNumber::formatTransfer(transferredIn)));

    ui->out_label->setText(QString("out: %1 (%2) [%3]").arg(FormatNumber::formatSpeed(currentSpeedOut),
                                                            FormatNumber::formatSpeed(maximumOut),
                                                            FormatNumber::formatTransfer(transferredOut)));
    ui->in_graph->repaint();
    ui->out_graph->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}
