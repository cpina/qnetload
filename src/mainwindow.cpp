#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formatnumber.h"

#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QMenu>
#include <QSettings>
#include <QWheelEvent>


/*
 * Copyright 2017, 2019, 2020 Carles Pina i Estany <carles@pina.cat>
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
    m_networkInformation(new NetworkInformationReader(this)),
    m_informationStorage(new InformationStorage(this)),
    m_timer(new QTimer(this)),
    m_resetWaitingForConfirmation(false),
    m_resetCountDown(0)
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    ui->setupUi(this);

    m_resetPixmap = QPixmap(":/icons/arrow-circle-135-left.png");
    m_pausePixmap = QPixmap(":/icons/control-pause.png");

    connect(ui->interface_name, &ClickableLabel::leftClicked,
            this, &MainWindow::selectNextInterface);

    QString interfaceSelectedName = interfaceName;

    setFontSize(readCurrentFontSize());

    ui->reset_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->pause_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->message->setText("");

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());

    connect(m_networkInformation, &NetworkInformationReader::interfaceNameChanged,
            m_informationStorage, &InformationStorage::initialize);
    connect(m_networkInformation, SIGNAL(interfaceNameChanged()),
            this, SLOT(interfaceNameChanged()));
    connect(ui->reset_button, &QAbstractButton::clicked,
            this, &MainWindow::reset);
    connect(ui->pause_button, &QAbstractButton::clicked,
            this, &MainWindow::togglePause);

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

    setTooltips();

    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateInformation);
    m_timer->start(1000);

    // Speed to zero to start with
    setAllLabels(m_networkInformation->interfaceName(), 0,
                 0, 0, 0,
                 0, 0, 0,
                 0);

    ui->in_graph->setType(InformationStorage::InOrOutType::InType);
    ui->in_graph->setInformationStorage(m_informationStorage);

    ui->out_graph->setType(InformationStorage::InOrOutType::OutType);
    ui->out_graph->setInformationStorage(m_informationStorage);

    updateInformation();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &MainWindow::showContextualMenu);
}

void MainWindow::reset()
{
    if (m_resetWaitingForConfirmation)
    {
        ui->message->setText("");
        m_informationStorage->initialize();
        updateInformation();
        m_resetWaitingForConfirmation = false;
        m_resetCountDown = 0;
    }
    else
    {
        m_resetCountDown = 5;
        m_resetWaitingForConfirmation = true;
        resetCountDown();
    }
}

void MainWindow::togglePause()
{
    InformationStorage::NetworkBytesInOutPaused currentInformation = m_networkInformation->readInformation();

    if (m_informationStorage->isPaused())
    {
        m_informationStorage->unpause(currentInformation);
        ui->message->setText("");
    }
    else
    {
        m_informationStorage->pause(currentInformation);
        ui->message->setText("Paused");
    }
}

void MainWindow::resetCountDown()
{
    if (m_resetCountDown == -1)
    {
        m_resetWaitingForConfirmation = false;
        m_resetCountDown = 0;
        ui->message->setText("");
        return;
    }

    if (!m_resetWaitingForConfirmation)
    {
        return;
    }

    ui->message->setText(tr("%1 click again").arg(m_resetCountDown));
    m_resetCountDown--;

    QTimer::singleShot(1000, this, &MainWindow::resetCountDown);
}

void MainWindow::interfaceNameChanged()
{
    QSettings settings;
    settings.setValue("latestInterfaceName", m_networkInformation->interfaceName());

    QTimer::singleShot(1, this, SLOT(updateInformation()));
}

void MainWindow::selectNextInterface()
{
    QString currentInterface = m_networkInformation->interfaceName();
    QStringList listOfInterfaces = m_networkInformation->listOfInterfaces();

    int newIndex = (listOfInterfaces.indexOf(currentInterface) + 1) % listOfInterfaces.count();

    QString newInterface = listOfInterfaces.at(newIndex);

    m_networkInformation->setInterfaceName(newInterface);
}

void MainWindow::showContextualMenu(const QPoint& position)
{
    QMenu* contextualMenu = new QMenu(this);

    QMenu* fontSizes = new QMenu(tr("Font Size"), contextualMenu);

    int currentFontSize = readCurrentFontSize();
    for(int fontSize = 5; fontSize < 15; fontSize++)
    {
        QAction* fontSizeAction = fontSizes->addAction(QString::number(fontSize));

        fontSizeAction->setCheckable(true);
        fontSizeAction->setChecked(currentFontSize == fontSize);

        connect(fontSizeAction, &QAction::triggered,
                this, [this, fontSize]{setFontSize(fontSize);} );
    }

    contextualMenu->addMenu(fontSizes);

    QMenu* interfaces = new QMenu(tr("Interfaces"), contextualMenu);

    for(QString interface : m_networkInformation->listOfInterfaces())
    {
        QAction* interfaceAction = interfaces->addAction(interface);
        interfaceAction->setCheckable(true);
        interfaceAction->setChecked(interface == m_networkInformation->interfaceName());

        if (NetworkInformationReader::isNotableInterface(interface))
        {
            QFont font = interfaceAction->font();
            font.setBold(NetworkInformationReader::isNotableInterface(interface));
            interfaceAction->setFont(font);
        }

        connect(interfaceAction, &QAction::triggered,
                this, [this, interface]{m_networkInformation->setInterfaceName(interface);} );
    }

    contextualMenu->addMenu(interfaces);

    QAction* version = contextualMenu->addAction(tr("Version: %1").arg(QApplication::applicationVersion()));
    version->setEnabled(false);

    contextualMenu->exec(mapToGlobal(position));
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    // Resizes fonts
    if (!(QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        return;
    }

    int y = event->angleDelta().y();

    int newFontSize;
    if (y>0)
    {
        newFontSize = readCurrentFontSize() + 1;
    }
    else
    {
        newFontSize = readCurrentFontSize() - 1;
    }

    if (newFontSize <= 0)
    {
        return;
    }

    int inGraphHeight = ui->in_graph->height();
    setFontSize(newFontSize);

    // Easy way to force the labels to be drawn to get the differential
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    int graphDelta = inGraphHeight - ui->in_graph->height();

    QSize newSize = size();
    QSize oldSize = size();

    newSize.setHeight(oldSize.height() + graphDelta * 2);
    newSize.setWidth(oldSize.width() + graphDelta * 2);

    resize(newSize);
}

int MainWindow::readCurrentFontSize() const
{
    QSettings settings;

    if (settings.value("fontSize").isValid())
    {
        return settings.value("fontSize").toInt();
    }
    return 9;
}

void MainWindow::setFontSize(int fontSize)
{
    for(QLabel* label : findChildren<QLabel*>())
    {
        QFont font = label->font();
        font.setPointSize(fontSize);
        label->setFont(font);
    }

    QSettings settings;
    settings.setValue("fontSize", fontSize);

    int labelHeight = ui->out_current_speed->height();
    ui->reset_button->setFixedSize(labelHeight, labelHeight);

    // Reset button resize
    int fontHeight = ui->interface_name->fontMetrics().height();
    QPixmap resetPixmapResized = m_resetPixmap.scaledToHeight(fontHeight);
    QPixmap pausePixmapResized = m_pausePixmap.scaledToHeight(fontHeight);

    updateButtonIcon(ui->reset_button, resetPixmapResized);
    updateButtonIcon(ui->pause_button, pausePixmapResized);
}

void MainWindow::updateButtonIcon(QToolButton* toolButton, const QPixmap& pixmap)
{
    toolButton->setIconSize(pixmap.size());
    toolButton->setIcon(pixmap);
    toolButton->setFixedSize(pixmap.size());
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

    ui->total_transferred->setToolTip(tr("Total (in and out) transferred data"));
    ui->reset_button->setToolTip(tr("Reset counters"));
    ui->pause_button->setToolTip(tr("Pause counters"));
}

void MainWindow::setAllLabels(const QString& interfaceName, quint64 millisecondsSinceStart,
                  quint64 currentSpeedIn, quint64 maximumSpeedIn, quint64 transferredIn,
                  quint64 currentSpeedOut, quint64 maximumSpeedOut, quint64 transferredOut,
                  quint64 transferredTotal)
{
    ui->interface_name->setText(interfaceName);
    ui->time_running->setText(FormatNumber::formatElapsedTime(millisecondsSinceStart));

    ui->in_current_speed->setText(FormatNumber::formatSpeed(currentSpeedIn));
    ui->in_maximum_speed->setText(QString("(%1)").arg(FormatNumber::formatSpeed(maximumSpeedIn)));
    ui->in_transferred->setText(QString("[%1]").arg(FormatNumber::formatTransfer(transferredIn)));

    ui->out_current_speed->setText(FormatNumber::formatSpeed(currentSpeedOut));
    ui->out_maximum_speed->setText(QString("(%1)").arg(FormatNumber::formatSpeed(maximumSpeedOut)));
    ui->out_transferred->setText(QString("[%1]").arg(FormatNumber::formatTransfer(transferredOut)));

    ui->total_transferred->setText(QString("%1").arg(FormatNumber::formatTransfer(transferredTotal)));
}

void MainWindow::updateInformation()
{
    NetworkInformationReader::NetworkBytesInOut information = m_networkInformation->readInformation();

    m_informationStorage->addInformation(information);

    quint64 maximumSpeedIn = m_informationStorage->maximumSpeedIn();
    quint64 maximumSpeedOut = m_informationStorage->maximumSpeedOut();

    quint64 transferredIn = m_informationStorage->transferredIn();
    quint64 transferredOut = m_informationStorage->transferredOut();
    quint64 transferredTotal = transferredIn + transferredOut;

    NetworkInformationReader::NetworkBytesInOut currentSpeed = m_informationStorage->currentSpeed();
    quint64 currentSpeedIn = currentSpeed.in;
    quint64 currentSpeedOut = currentSpeed.out;

    setAllLabels(m_networkInformation->interfaceName(), m_informationStorage->millisecondsSinceStart(),
                 currentSpeedIn, maximumSpeedIn, transferredIn,
                 currentSpeedOut, maximumSpeedOut, transferredOut,
                 transferredTotal);

    ui->in_graph->repaint();
    ui->out_graph->repaint();
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    delete ui;
}
