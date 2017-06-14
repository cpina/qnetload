#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formatnumber.h"

#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QSettings>


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

MainWindow::MainWindow(const QString& interfaceName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_networkInformation(0),
    m_informationStorage(new InformationStorage(this)),
    m_timer(0)
{
    ui->setupUi(this);

    QString interfaceSelectedName = interfaceName;

    m_networkInformation = new NetworkInformationReader(this);

    QStringList listOfInterfaces = m_networkInformation->listOfInterfaces();

    if (listOfInterfaces.isEmpty())
    {
        QString message = tr("qnetload cannot read any interface from %1").arg(m_networkInformation->procNetDev());
        QMessageBox::critical(this, tr("qnetload"), message);
        exit(2);
    }

    if (interfaceSelectedName.isEmpty())
    {
        interfaceSelectedName = chooseInterfaceName();
    }

    if (!listOfInterfaces.contains(interfaceSelectedName))
    {
        QString message1 = tr("Passed interface %1 not found in %2").arg(interfaceSelectedName)
                                                                    .arg(m_networkInformation->procNetDev());
        QString message2 = tr("qnetload will choose another interface");

        QMessageBox::warning(this, tr("qnetload"), message1 + "\n" + message2);
        interfaceSelectedName = chooseInterfaceName();
    }

    m_networkInformation->setInterfaceName(interfaceSelectedName);

    QSettings settings;
    settings.setValue("latestInterfaceName", interfaceSelectedName);
    setTooltips();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInformation()));
    m_timer->start(1000);

    // Speed to zero to start with
    setAllLabels(m_networkInformation->interfaceName(), 0,
                 0, 0, 0,
                 0, 0, 0);

    ui->in_graph->setType(InformationStorage::InType);
    ui->in_graph->setInformationStorage(m_informationStorage);

    ui->out_graph->setType(InformationStorage::OutType);
    ui->out_graph->setInformationStorage(m_informationStorage);

    updateInformation();
}

QString MainWindow::chooseInterfaceName() const
{
    QSettings settings;

    if (settings.value("latestInterfaceName").isValid())
    {
        return settings.value("latestInterfaceName").toString();
    }
    else
    {
        return m_networkInformation->chooseInterfaceFromProcNetDevInterface();
    }
}

void MainWindow::setTooltips()
{
    ui->interface_name->setToolTip(tr("Monitored interface"));
    ui->time_running->setToolTip(tr("Time elapsed since qnetload was started"));

    ui->in_current_speed->setToolTip(tr("Speed receiving data"));
    ui->in_maximum_speed->setToolTip(tr("Maximum speed receiving data (intervals of 1 second)"));
    ui->in_transferred->setToolTip(tr("Total amount of data received since qnetload was started"));

    ui->out_current_speed->setToolTip(tr("Speed sending data"));
    ui->out_maximum_speed->setToolTip(tr("Maximum speed sending data (intervals of 1 second)"));
    ui->out_transferred->setToolTip(tr("Total amount of data sent since qnetload was started"));
}

void MainWindow::setAllLabels(const QString& interfaceName, quint64 millisecondsSinceStart,
                  quint64 currentSpeedIn, quint64 maximumSpeedIn, quint64 transferredIn,
                  quint64 currentSpeedOut, quint64 maximumSpeedOut, quint64 transferredOut)
{
    ui->interface_name->setText(interfaceName);
    ui->time_running->setText(FormatNumber::formatElapsedTime(millisecondsSinceStart));

    ui->in_current_speed->setText(FormatNumber::formatSpeed(currentSpeedIn));
    ui->in_maximum_speed->setText(QString("(%1)").arg(FormatNumber::formatSpeed(maximumSpeedIn)));
    ui->in_transferred->setText(QString("[%1]").arg(FormatNumber::formatTransfer(transferredIn)));

    ui->out_current_speed->setText(FormatNumber::formatSpeed(currentSpeedOut));
    ui->out_maximum_speed->setText(QString("(%1)").arg(FormatNumber::formatSpeed(maximumSpeedOut)));
    ui->out_transferred->setText(QString("[%1]").arg(FormatNumber::formatTransfer(transferredOut)));
}

void MainWindow::updateInformation()
{
    NetworkInformationReader::NetworkBytesInOut information = m_networkInformation->readInformation();

    m_informationStorage->addInformation(information);

    quint64 maximumSpeedIn = m_informationStorage->maximumSpeedIn();
    quint64 maximumSpeedOut = m_informationStorage->maximumSpeedOut();

    quint64 transferredIn = m_informationStorage->transferredIn();
    quint64 transferredOut = m_informationStorage->transferredOut();

    NetworkInformationReader::NetworkBytesInOut currentSpeed = m_informationStorage->currentSpeed();
    quint64 currentSpeedIn = currentSpeed.in;
    quint64 currentSpeedOut = currentSpeed.out;

    setAllLabels(m_networkInformation->interfaceName(), m_informationStorage->millisecondsSinceStart(),
                 currentSpeedIn, maximumSpeedIn, transferredIn,
                 currentSpeedOut, maximumSpeedOut, transferredOut);

    ui->in_graph->repaint();
    ui->out_graph->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}
