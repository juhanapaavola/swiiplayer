/* 
 * BTController - Connecting WiiMote to Symbian phone
 * Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>
 * 
 * This file is part of BTController.
 * 
 * BTController is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BTController is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTController.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "BTController.h"

CBTController::CBTController(): iControllerAmount(0),
        iReDiscover(EFalse)        
    {
    }

CBTController* CBTController::NewLC()
    {
    CBTController* self = new (ELeave) CBTController();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CBTController* CBTController::NewL()
    {
    CBTController* self = CBTController::NewLC();
    CleanupStack::Pop();
    return self;
    }

void CBTController::ConstructL()
    {
    iState = EControllerStarting;        
    iUserAskedPower = EFalse;
    iPowerQuery = CBTPowerQuery::NewL(*this);
    iDeviceFinder = CDeviceFinder::NewL(*this);
    iConnections = CConnectionData::NewL(*this);
    }

CBTController::~CBTController()
    {
    if(iPowerQuery)
        {
        delete iPowerQuery;
        iPowerQuery = NULL;
        }
    if(iDeviceFinder)
        {
        delete iDeviceFinder;
        iDeviceFinder = NULL;
        }
    if(iConnections)
        {
        delete iConnections;
        iConnections = NULL;        
        }
    }

void CBTController::StartL(MBTControllerServerSessionCallbacks* aServerSession)
    {
    if(iState == EControllerStarting)
        {
        iServerSession = aServerSession;
        iState = EControllerStarted;
        }
    }

void CBTController::Stop()
    {
    if(iPowerQuery)
        {
        delete iPowerQuery;
        iPowerQuery = NULL;
        }
    if(iDeviceFinder)
        {
        delete iDeviceFinder;
        iDeviceFinder = NULL;
        }
    if(iConnections)
        {
        delete iConnections;
        iConnections = NULL;        
        }        
    }

void CBTController::PowerOn()
    {
    iUserAskedPower = ETrue;
    if(!iPower)
        {
        iPowerQuery->TurnBtOn();
        }
    else
        {
        iPowerQuery->PowerReply();
        }
    }

void CBTController::PowerState(TBool aPower)
    {   
    iPower = aPower;
    if(iState == EControllerStarted && iUserAskedPower)
        {
        iUserAskedPower = EFalse;
        if(aPower)
            {        
            iServerSession->Powered(KErrNone);
            }
        else
            {
            iServerSession->Powered(KErrNotReady);
            }    
        }
    }

void CBTController::StartDiscoverL()
    {
    if(!iPower)
        {
        iServerSession->ControllerDiscoverComplete(KErrNotReady,iAddr);
        }
    else
        {
        TRemoteInfo info;
        info = iConnections->RemoteInfo();
        if(info.iConnected)
            {
            iReDiscover = ETrue;
            iConnections->Disconnect();
            }
        else
            {
            iDeviceFinder->StartDiscoveringL();
            }
        }
    }

void CBTController::CancelDiscovery()
    {
    iDeviceFinder->Cancel();
    }

void CBTController::DeviceSelected(TBTDevAddr aAddr, TControllerDataTypes::TRemoteType aType, TInt aError)
    {
    iAddr = aAddr;
    if(aError == KErrNone)
        {
        TRemoteInfo info;
        info = iConnections->RemoteInfo();
        info.iType = aType;
        iConnections->UpdateRemoteInfo(info);
        }    
    iServerSession->ControllerDiscoverComplete(aError,iAddr);
    }

void CBTController::ServiceSearchFinished(TInt aError)
    {    
    if(aError == KErrNone)
        {
        iDeviceFinder->Cancel();
        iServerSession->ControllerDiscoverComplete(aError,iAddr);
        }
    else
        {
        iDeviceFinder->ContinueDiscover();
        }
    }

void CBTController::SetSocketSecurity(TBTServiceSecurity aSecurity, TInt aType)
    {
    if(aType == (TInt)TControllerDataTypes::EChannelTypeInterrupt)
        {
        iInterruptSecurity = aSecurity;
        }
    if(aType == (TInt)TControllerDataTypes::EChannelTypeControl)
        {
        iControlSecurity = aSecurity;
        }
    }

void CBTController::ConnectDevice(TL2CAPSockAddr aAddr)
    {
    TBuf<255> tmp;
    aAddr.BTAddr().GetReadable(tmp,_L("BT ADDR: "),_L(":"),KNullDesC);
    RDebug::Print(tmp);

    if(iState != EControllerStarted)
        {
        iServerSession->ConnectToComplete(KErrHardwareNotAvailable);
        }
    else
        {
        TInt err(0);
        err = iConnections->AddConnectionL(aAddr);
        switch(err)
            {
            case KErrAlreadyExists:
                {
                if(iConnections->ConnectionExists(aAddr))
                    {
                    if(!iConnections->Connection().Connected() )
                        {
                        iConnections->ReConnect();
                        }
                    }
                }break;
            case KErrNone:
                {
                iConnections->ConnectL(aAddr);
                }break;
            default:
                iConnections->Remove();
                iServerSession->ConnectToComplete(KErrAlreadyExists);            
                break;
            }        
        }
    }

void CBTController::ConnectDeviceComplete(TInt aError)
    {
    iServerSession->ConnectToComplete(aError);
    }

void CBTController::DisconnectDevice()
    {    
    iConnections->Disconnect();
    }

void CBTController::DisconnectDeviceComplete(TInt aError)
    {    
    if(aError != KErrNone)
        {
        iServerSession->ErrorComplete(aError);
        }
    
    if(iReDiscover)
        {
        iDeviceFinder->StartDiscoveringL();
        iReDiscover = EFalse;
        }   
    }

void CBTController::LoadKeyConfig(const TDesC& aFilename)
    {
    iConnections->Parser().LoadKeyConfig(aFilename);
    }

void CBTController::KeyConfigReady(TInt aError)
    {
    iServerSession->KeyConfigLoadComplete(aError);
    }

void CBTController::GetControllerType(TControllerDataTypes::TRemoteType& aType)
    {
    TRemoteInfo info;
    info = iConnections->RemoteInfo();
    
    if(info.iConnected)
        {
        aType = info.iType;
        }
    else
        {
        aType = TControllerDataTypes::ERemoteTypeNoController;
        }
    }

TInt CBTController::GetBatteryStatus()
    {
    TRemoteInfo info;
    TInt ret(KErrNone);
    info = iConnections->RemoteInfo();
    
    if(info.iConnected)
        {
        info.iCommand = (TUint8)TControllerDataTypes::EWiiMoteCommandControlStatus;
        iConnections->CommandCreator().SendSettingWii(info);
        }
    else
        {
        ret = KErrGeneral;
        }
    return ret;
    }

TInt CBTController::EnableWiiAcc(TBool aEnable)
    {
    TRemoteInfo info;
    TInt ret(KErrNone);
    info = iConnections->RemoteInfo();
    info.iCommand = (TUint8)TControllerDataTypes::EWiiMoteCommandReportType;
    if(info.iConnected)
        {
        info.iWiiMotion = aEnable;
        iConnections->CommandCreator().SendSettingWii(info);
        }
    else
        {
        ret = KErrGeneral;
        }
    return ret;
    }

void CBTController::SendUpdate(TRemoteInfo aInfo)
    {
    switch(aInfo.iCommand)
        {
        case (TUint8)TControllerDataTypes::EWiiMoteReportControlStatus:            
            {
            iServerSession->GetBatteryStatusComplete(aInfo.iBattery,KErrNone);
            iServerSession->GetSpeakerEnabledComplete(aInfo.iWiiSpeakerEnabled,KErrNone);            
            }break;        
        case (TUint8)TControllerDataTypes::EWiiMoteCommandAudioEnable:
        case (TUint8)TControllerDataTypes::EWiiMoteCommandAudioMute:
            {
            TInt err(0);
            if(aInfo.iWiiSpeakerEnabled == TControllerDataTypes::EWiiSpeakerError)
                {
                err = KErrGeneral;
                }
            iServerSession->GetSpeakerEnabledComplete(aInfo.iWiiSpeakerEnabled,err);
            }break;
        default:
            break;
        }
    TRemoteInfo info;
    info = aInfo;
    info.iCommand = (TUint8)TControllerDataTypes::EWiiMoteCommandNone;
    iConnections->UpdateRemoteInfo(info);
    }

TInt CBTController::GetWiiSpeakerStatus()
    {
    TRemoteInfo info;
    TInt ret(KErrNone);
    info = iConnections->RemoteInfo();
    if(info.iConnected && info.iType == TControllerDataTypes::ERemoteTypeWii)
        {
        info.iCommand = (TUint8)TControllerDataTypes::EWiiMoteCommandControlStatus;
        iConnections->CommandCreator().SendSettingWii(info);
        }
    else
        {
        ret = KErrGeneral;
        }
    return ret;
    }

TInt CBTController::SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aState)
    {
    TRemoteInfo info;
    TInt ret(KErrNone);
    info = iConnections->RemoteInfo();
    if(info.iConnected && info.iType == TControllerDataTypes::ERemoteTypeWii)
        {
        info.iWiiSpeakerEnabled = aState;
        switch(aState)
            {
            case TControllerDataTypes::EWiiSpeakerEnabled:
            case TControllerDataTypes::EWiiSpeakerDisabled:
                {
                info.iCommand = TControllerDataTypes::EWiiMoteCommandAudioEnable;
                iConnections->CommandCreator().SendSettingWii(info);
                }break;
            case TControllerDataTypes::EWiiSpeakerMute:
                {
                info.iCommand = TControllerDataTypes::EWiiMoteCommandAudioMute;                
                iConnections->CommandCreator().SendSettingWii(info);
                }break;
            default:
                break;
            }
        }
    else
        {
        ret = KErrGeneral;
        }
    return ret;
    }


void CBTController::SetLoadByUid(TBool aLoadByUid)
    {
    TRemoteInfo info;
    info = iConnections->RemoteInfo();
    info.iLoadConfigByUid = aLoadByUid;
    info.iCommand = (TUint8)TControllerDataTypes::EWiiMoteCommandNone;
    iConnections->UpdateRemoteInfo(info);
    }
	
void CBTController::GetConfigName(TFileName& aFilename)
    {
    iConnections->Parser().GetConfigName(aFilename);
    }
