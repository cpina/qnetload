#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QMessageBox>

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("qnetload");
    QApplication::setApplicationName("qnetload");
    QApplication::setApplicationVersion("1.2");

    qInfo() << qUtf8Printable("qnetload version " + QApplication::applicationVersion() + " Copyright (C) 2017, 2019, 2020 Carles Pina i Estany <carles@pina.cat>");
    qInfo() << qUtf8Printable("This program comes with ABSOLUTELY NO WARRANTY");
    qInfo() << qUtf8Printable("This is free software, and you are welcome to redistribute it under certain conditions");
    qInfo() << qUtf8Printable("License: GPL v3");
    qInfo() << qUtf8Printable("Icons from https://p.yusukekamiyamane.com");

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
