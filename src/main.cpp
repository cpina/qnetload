#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("qnetload");
    QApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Display traffic for a network interface.\n\nqnetload Copyright (C) 2017 Carles Pina i Estany <carles@pina.cat>\nLicense: GPLv3");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("interface", QCoreApplication::translate("interface", "Interface to monitor (e.g. eth0, wlan0, ...)."));

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    QString interfaceName;
    QString helpText = parser.helpText();

    if (args.count() == 1)
    {
        interfaceName = args[0];
    }

    MainWindow w(interfaceName, helpText);
    w.show();

    return app.exec();
}
