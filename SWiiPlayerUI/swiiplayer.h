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

#ifndef SWIIPLAYER_H
#define SWIIPLAYER_H

#include <QMainWindow>

namespace Ui {
    class SWiiPlayer;
}

class ControllerWrapper;
class QCloseEvent;
class QTimer;

class SWiiPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit SWiiPlayer(QWidget *parent = 0);
    ~SWiiPlayer();

protected:
    void closeEvent(QCloseEvent* ce);

private:
    Ui::SWiiPlayer *ui;

private slots:
    void on_helpButton_clicked();
    void on_configureButton_clicked();
    void on_disconnectButton_clicked();
    void on_connectButton_clicked();

    void slotDeviceConnected(int error);
    void slotUpdateBatteryStatus(int level);
    void slotCancelServer();
    void slotPowerFinished(int error);
    void slotKeyConfigLoaded(int error);
    void slotSetLoadByUid(bool load);
    void slotGetBattery();

private:
    void setConnected(bool connected);
    void connectController();

private:
    ControllerWrapper*  m_wrapper;
    bool                mLoadByUid;
    bool                mConnected;
    QTimer*             mTimer;
};

#endif // SWIIPLAYER_H
