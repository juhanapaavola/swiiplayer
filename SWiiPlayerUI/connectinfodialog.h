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

#ifndef CONNECTINFODIALOG_H
#define CONNECTINFODIALOG_H

#include <QDialog>

namespace Ui {
    class ConnectInfoDialog;
}

class ConnectInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectInfoDialog(QWidget *parent = 0);
    ~ConnectInfoDialog();

private:
    Ui::ConnectInfoDialog *ui;

};

#endif // CONNECTINFODIALOG_H
