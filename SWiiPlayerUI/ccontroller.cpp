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

#include "ccontroller.h"

CController::CController(ControllerWrapper &aPublicAPI): CActive(EPriorityStandard),
iPublicAPI(aPublicAPI),
iState(ENothing),
iConnected(EFalse)
{
    CActiveScheduler::Add(this);
}

CController* CController::NewL(ControllerWrapper &aPublicAPI)
{
    CController* self = new CController(aPublicAPI);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CController::~CController()
{
    Cancel();
    iClient.Close();
}

void CController::DoCancel()
{
    iClient.CancelAll();
    try {
        emit iPublicAPI.controllerCanceled();
    } catch(std::exception&){}
    iState = ENothing;
}

void CController::RunL()
{
    switch(iState)
    {
    case EFindAndConnect:
        {
            if(iStatus.Int()==KErrNone)
            {
                iConnected = ETrue;
            }else{
                iState = ENothing;
            }
            QT_TRYCATCH_LEAVING( emit iPublicAPI.deviceConnected(iStatus.Int()) );
        }break;
    case EBatteryStatus:
        {
        if(iStatus.Int() != KErrNone){
            iBattery = iStatus.Int();
            iState = ENothing;
            iConnected = EFalse;
        }
        QT_TRYCATCH_LEAVING( emit iPublicAPI.batteryStatus(iBattery));
        }break;
    case ERemoteType:
        {
        if(iStatus.Int() == KErrNone)
            {
            QT_TRYCATCH_LEAVING( emit iPublicAPI.remoteType((TInt)iType));
            }
        else
            {
            iState = ENothing;
            }
        }break;
    case EPower:
        {
        QT_TRYCATCH_LEAVING( emit iPublicAPI.powerFinished(iStatus.Int()) );
        iState = ENothing;
        }break;
    case ELoadConfig:
        {
        if(iFilename)
        {
            delete iFilename;
            iFilename = NULL;
        }
        QT_TRYCATCH_LEAVING( emit iPublicAPI.keyConfigLoaded(iStatus.Int()));
        }break;

    default:
        break;
    }
}

void CController::ConstructL()
{
    TInt err(KErrNone);
    err = iClient.Connect();
    User::LeaveIfError(err);

    iClient.Power();
}

void CController::findAndConnect()
{
    Cancel();
    iState = EFindAndConnect;
    iClient.FindAndConnect(iStatus);
    SetActive();
}

void CController::getBatteryStatus()
{
    if(IsActive())
        return;
    iState = EBatteryStatus;
    iClient.BatteryStatus(iBattery,iStatus);
    SetActive();
}

void CController::getRemoteType()
    {    
    if(IsActive())
        return;
    iState = ERemoteType;
    iClient.ControllerType(iType,iStatus);
    SetActive();
    }

void CController::disconnectRemote()
    {
    if(IsActive())
        {
        Cancel();
        }
    iClient.Disconnect();
    iConnected = EFalse;
    }

void CController::Power()
{
    if(IsActive())
        return;
    iState = EPower;
    iClient.Power(iStatus);
    SetActive();
}

void CController::LoadConfig(const TDesC& aName)
{
    TFileName fn;
    fn.Copy(aName);
    if(IsActive())
        return;

    iState = ELoadConfig;
    if(iFilename)
    {
        delete iFilename;
        iFilename = NULL;
    }
    iFilename = fn.AllocL();
    iClient.LoadKeyConfig(fn,iStatus);
    SetActive();
}

void CController::LoadConfigByUid(TBool aLoadByUid)
{
    if(IsActive() || iConnected){
        iClient.CancelAll();
    }
    iClient.SetLoadConfigByUid(aLoadByUid ? ETrue : EFalse);
}

void CController::GetCurrentConfigName(TFileName& aFilename)
{
    if(IsActive()){
        iClient.CancelAll();
    }
    iClient.GetCurrentConfigName(aFilename);
}
