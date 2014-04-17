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

#include "configuredialog.h"
#include "ui_configuredialog.h"
#include <QDir>
#include <QFileDialog>

const QString DATADIR = "/WiiControllerConfig/";

ConfigureDialog::ConfigureDialog(bool loadByUid,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);    
    ui->loadByUIDButton->setChecked(loadByUid);
#if defined(Q_OS_SYMBIAN)
    this->showMaximized();
#else
    this->show();
#endif
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::on_loadByUIDButton_clicked()
{
    if(ui->loadByUIDButton->isChecked())
        emit loadByUID(true);
    else
        emit loadByUID(false);
}

void ConfigureDialog::on_loadConfigButton_clicked()
{
    QString p;
#ifndef _SIMULATOR_STUB
    p = QDir::rootPath();
#else
    p = QDir::homePath();
#endif
    p.append(DATADIR);

    QString fname = QFileDialog::getOpenFileName(this,tr("Open config"),p,QString("xml (*.xml)"));
    if(fname.endsWith(".xml")){
        emit loadKeyConfig(fname);
    }
}
