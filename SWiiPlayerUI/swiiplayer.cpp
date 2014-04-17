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

#include "SWiiPlayer.h"
#include "ui_swiiplayer.h"
#include "controllerwrapper.h"
#include <QMessageBox>
#include "waitingdialog.h"
#include "connectinfodialog.h"
#include <QCloseEvent>
#include "configuredialog.h"
#include "helpdialog.h"
#include <QTimer>

SWiiPlayer::SWiiPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SWiiPlayer)
{
    ui->setupUi(this);
    ui->batteryBar->hide();

    m_wrapper = new ControllerWrapper(this);
    connect(m_wrapper,SIGNAL(deviceConnected(int)),this,SLOT(slotDeviceConnected(int)));
    connect(m_wrapper,SIGNAL(batteryStatus(int)),this,SLOT(slotUpdateBatteryStatus(int)));
    connect(m_wrapper,SIGNAL(powerFinished(int)),this,SLOT(slotPowerFinished(int)));
    connect(m_wrapper,SIGNAL(keyConfigLoaded(int)),this,SLOT(slotKeyConfigLoaded(int)));

    mTimer = new QTimer(this);    
    connect(mTimer,SIGNAL(timeout()),this,SLOT(slotGetBattery()));
    setConnected(false);
}

SWiiPlayer::~SWiiPlayer()
{
    delete ui;
}

void SWiiPlayer::on_connectButton_clicked()
{
    slotCancelServer();
    m_wrapper->disconnectRemote();
    m_wrapper->power();
}

void SWiiPlayer::on_disconnectButton_clicked()
{
    if(QMessageBox::question(this,tr("Disconnect"),
                             tr("Do you want to disconnect ?"),
                             QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
        m_wrapper->disconnectRemote();
        slotCancelServer();
    }
}

void SWiiPlayer::on_configureButton_clicked()
{
    ConfigureDialog cd(mLoadByUid);
    connect(&cd,SIGNAL(loadByUID(bool)),m_wrapper,SLOT(setLoadByUid(bool)));
    connect(&cd,SIGNAL(loadByUID(bool)),this,SLOT(slotSetLoadByUid(bool)));
    connect(&cd,SIGNAL(loadKeyConfig(QString)),m_wrapper,SLOT(loadKeyConfig(QString)));
    cd.exec();
}

void SWiiPlayer::on_helpButton_clicked()
{
    HelpDialog hd;
    hd.exec();
}

void SWiiPlayer::setConnected(bool connected)
{
    if(connected){
        ui->batteryBar->show();
        ui->connectButton->hide();
        ui->disconnectButton->show();
        mTimer->start(2000);
    }else{
        ui->batteryBar->hide();
        ui->connectButton->show();
        ui->disconnectButton->hide();
        mTimer->stop();
    }
    mConnected = connected;
}

void SWiiPlayer::slotDeviceConnected(int error)
{
    if( error < 0){
        switch(error){
        case -18:{
                // no power on BT
                QMessageBox::warning(this,tr("Error"),tr("Bluetooth power failure"));
            }break;
        default:
            QMessageBox::warning(this,tr("Error"),tr("Could not find Wiimote"));
            break;
        }
        m_wrapper->cancelAll();
        m_wrapper->disconnectRemote();
        setConnected(false);
    }else{
        setConnected(true);
        m_wrapper->setLoadByUid(mLoadByUid);
        QMessageBox::information(this,tr("Connected"),tr("Device connected"),QMessageBox::Close);
    }
}

void SWiiPlayer::slotUpdateBatteryStatus(int level)
{
    if(level < 0){
        slotCancelServer();
        QMessageBox::information(this,tr("Disconnected"),tr("Remote disappeared"));
        m_wrapper->disconnectRemote();
        setConnected(false);
    }else{
        ui->batteryBar->setValue(level);
        setConnected(true);
    }
}

void SWiiPlayer::slotCancelServer()
{
    m_wrapper->cancelAll();
    m_wrapper->disconnectRemote();
    setConnected(false);
}

void SWiiPlayer::connectController()
{
    ConnectInfoDialog info;
    info.exec();
    if(info.result() == QDialog::Rejected)
        return;

    m_wrapper->connectController();
    WaitingDialog wd;
    connect(m_wrapper,SIGNAL(deviceConnected(int)),&wd,SLOT(slotCloseDialog(int)));
    wd.exec();

    if(wd.result() == QDialog::Rejected)
        slotCancelServer();
}

void SWiiPlayer::slotPowerFinished(int error)
{
    if(!error){
        connectController();
    }else{
        QMessageBox::warning(this,tr("Problem"),tr("Bluetooth not enabled"));
    }
}

void SWiiPlayer::closeEvent(QCloseEvent* ce)
{
    if(QMessageBox::question(this,tr("Exit"),
                             tr("Do you want exit application?"),
                             QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        ce->accept();
    else
        ce->ignore();
}

void SWiiPlayer::slotKeyConfigLoaded(int error)
{
    if(error<0)
        QMessageBox::critical(this,tr("Load error"),tr("Cannot load key config"),QMessageBox::Close);
}

void SWiiPlayer::slotSetLoadByUid(bool load)
{
    mLoadByUid = load;
}

void SWiiPlayer::slotGetBattery()
{
    m_wrapper->getBatteryStatus();
}
