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

#ifndef CONTROLLERWRAPPER_H
#define CONTROLLERWRAPPER_H

#include <QObject>
#ifdef Q_OS_SYMBIAN
#include <TControllerDataTypes.h>
#endif

class CController;

class ControllerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ControllerWrapper(QObject *parent = 0);
    virtual ~ControllerWrapper();
    
signals:
    /*
     deviceConnected(int error)

     1: wiimote found and connected, no errors
     <0: Symbian error codes
     */
    void deviceConnected(int error);

    /*
     batteryStatus(int batteryStatus)

     shows battery level or Symbian error code
     */
    void batteryStatus(int batteryStatus);

    void controllerCanceled();
    void remoteType(int type);

    /*
     powerFinished(int error)

     Phone BT power reply, Symbian error codes
     */
    void powerFinished(int error);
    
    void keyConfigLoaded(int error);


public slots:

    void connectController();

    /*
     getBatteryStatus()

     query wiimote battery level, maybe need remove this as after connection we start heartbeat
     */
    void getBatteryStatus();
    void getRemoteType();
    void cancelAll();
    void disconnectRemote();
    void power();
    void loadKeyConfig(QString name);
    void setLoadByUid(bool loadByUid);

public:
    QString getCurrentConfigName();

private:
    CController*    d_ptr;
    
private:
    friend class CController;
};

#endif // CONTROLLERWRAPPER_H
