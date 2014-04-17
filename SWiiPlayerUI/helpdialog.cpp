/*
 * SWiiPlayerUI - Connecting WiiMote to Symbian phone
 * Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>
 *
 * This file is part of SWiiPlayerUI.
 *
 * SWiiPlayerUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SWiiPlayerUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with SWiiPlayerUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QFile>
#include <QDir>
#include "helpviewerdialog.h"

const QString LICENSE_FILE("LICENSE.LGPL");
const QString UIDCONF_FILE("uidconf.txt");
const QString LOADCONF_FILE("loadconf.txt");
const QString README_FILE("about.txt");

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
#if defined(Q_OS_SYMBIAN)
    this->showMaximized();
#else
    this->show();
#endif
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_uidButton_clicked()
{    
    QString fn = QApplication::applicationDirPath();
    fn.append("/"+UIDCONF_FILE);
    QFile file(fn);
    QString t;
    bool err = file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(err){
        t = file.readAll();
    }else{
        t = tr("Help not available");
    }
    file.close();
    HelpViewerDialog hvd;
    hvd.setText(tr("By UID loading"),t);
    hvd.exec();
}

void HelpDialog::on_configButton_clicked()
{
    QString fn = QApplication::applicationDirPath();
    fn.append("/"+LOADCONF_FILE);
    QFile file(fn);
    QString t;
    bool err = file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(err){
        t = file.readAll();
    }else{
        t = tr("Help not available");
    }
    file.close();
    HelpViewerDialog hvd;
    hvd.setText(tr("Loading config"),t);
    hvd.exec();
}

void HelpDialog::on_aboutButton_clicked()
{
    QString fn = QApplication::applicationDirPath();
    fn.append("/"+README_FILE);
    QFile file(fn);
    QString t;
    bool err = file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(err){
        t = file.readAll();
    }else{
        t = tr("Help not available");
    }
    file.close();
    HelpViewerDialog hvd;
    hvd.setText(tr("About"),t);
    hvd.exec();
}

void HelpDialog::on_licenseButton_clicked()
{
    QString fn = QApplication::applicationDirPath();
    fn.append("/"+LICENSE_FILE);
    QFile file(fn);
    QString t;
    bool err = file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(err){
        t = file.readAll();
    }else{
        t = tr("Help not available");
    }
    file.close();
    HelpViewerDialog hvd;
    hvd.setText(tr("License"),t);
    hvd.exec();
}
