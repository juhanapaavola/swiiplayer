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

#include "controllerwrapper.h"
#ifdef Q_OS_SYMBIAN
#include "ccontroller.h"
#endif

ControllerWrapper::ControllerWrapper(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(d_ptr = CController::NewL(*this) );
#elif defined(_SIMULATOR_STUB)
    // STUB for other platforms
#endif
}

ControllerWrapper::~ControllerWrapper()
    {
#ifdef Q_OS_SYMBIAN
    delete d_ptr;
#endif
    }


void ControllerWrapper::connectController()
{
#ifdef Q_OS_SYMBIAN
    //d_ptr->connectController();
    d_ptr->findAndConnect();
#elif defined(_SIMULATOR_STUB)
    emit deviceConnected(0);
#endif
}

void ControllerWrapper::getBatteryStatus()
{
#ifdef Q_OS_SYMBIAN
    d_ptr->getBatteryStatus();
#elif defined(_SIMULATOR_STUB)
    emit batteryStatus(50);
#endif
}

void ControllerWrapper::getRemoteType()
    {
#ifdef Q_OS_SYMBIAN
    d_ptr->getRemoteType();
#elif defined(_SIMULATOR_STUB)
    emit remoteType(0);
#endif
    }

void ControllerWrapper::cancelAll()
{
#ifdef Q_OS_SYMBIAN
    d_ptr->DoCancel();
#endif
}

void ControllerWrapper::disconnectRemote()
{
#ifdef Q_OS_SYMBIAN
    d_ptr->disconnectRemote();
#elif defined(_SIMULATOR_STUB)
#endif
}

void ControllerWrapper::power()
{
#ifdef Q_OS_SYMBIAN
    d_ptr->Power();
#elif defined(_SIMULATOR_STUB)
    emit powerFinished(0);
#endif
}

void ControllerWrapper::loadKeyConfig(QString name)
{
#ifdef Q_OS_SYMBIAN    
    QString str = name;
    str = str.replace("/","\\");
    str = str.trimmed();
    TFileName fn(str.utf16());
    d_ptr->LoadConfig(fn);
#elif defined(_SIMULATOR_STUB)
    emit keyConfigLoaded(0);
#endif
}

void ControllerWrapper::setLoadByUid(bool loadByUid)
{
#ifdef Q_OS_SYMBIAN
    d_ptr->LoadConfigByUid(loadByUid);
#endif
}

QString ControllerWrapper::getCurrentConfigName()
{
#ifdef Q_OS_SYMBIAN
    TFileName fn;
    d_ptr->GetCurrentConfigName(fn);
    return QString((QChar*)fn.Ptr(),fn.Length());
#elif defined(_SIMULATOR_STUB)
    return QString("");
#endif
}
