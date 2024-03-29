#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>

#include <iostream>

/*
 * Copyright 2017, 2019, 2020, 2023 Carles Pina i Estany <carles@pina.cat>
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("qnetload");
    QApplication::setApplicationName("qnetload");
    QApplication::setApplicationVersion("1.3.6");

    QTextStream stdoutStream(stdout);

    stdoutStream << "qnetload version " + QApplication::applicationVersion() + " Copyright (C) 2017, 2019, 2020, 2021, 2023 Carles Pina i Estany <carles@pina.cat>" << Qt::endl;
    stdoutStream << "This program comes with ABSOLUTELY NO WARRANTY" << Qt::endl;
    stdoutStream << "This is free software, and you are welcome to redistribute it under certain conditions" << Qt::endl;
    stdoutStream << "License: GPLv3+" << Qt::endl;
    stdoutStream << "Icons from https://p.yusukekamiyamane.com" << Qt::endl;

    QCommandLineParser parser;
    parser.setApplicationDescription("Display traffic for a network interface.");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption interfaceOption(QStringList() << "i" << "interface",
                                       QObject::tr("Interface to monitor. Defaults to the one used previously."), "interface");

    parser.addOption(interfaceOption);

    parser.process(app);

    QString interfaceName = parser.value(interfaceOption);

    MainWindow w(interfaceName);
    w.show();

    return app.exec();
}
