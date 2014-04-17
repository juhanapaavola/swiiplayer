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

#include "helpviewerdialog.h"
#include "ui_helpviewerdialog.h"

HelpViewerDialog::HelpViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpViewerDialog)
{
    ui->setupUi(this);
#if defined(Q_OS_SYMBIAN)
    this->showMaximized();
#else
    this->show();
#endif
}

HelpViewerDialog::~HelpViewerDialog()
{
    delete ui;
}

void HelpViewerDialog::setText(QString title, QString text)
{
    this->setWindowTitle(title);
    ui->textEdit->setText(text);    
}
