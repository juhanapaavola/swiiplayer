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
#include "ConnectionData.h"
#include <coemain.h>
#include <eikenv.h>
#include <APGWGNAM.H>
#include <APGTASK.H>  
#include <e32cmn.h>
    
#ifdef _USE_PS_ACC
static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
static _LIT_SECURITY_POLICY_C1(KWriteUserDataPolicy,ECapabilitySwEvent);    
#endif

CConnectionData::CConnectionData(MBTControlCallbacks& aObserver):
    iObserver(aObserver),
    iSentConnectionStatus(EFalse),
    iCanSend(ETrue)
    {
    }

CConnectionData::~CConnectionData()
    {
    if(iSocketConnector)
        {
        delete iSocketConnector;
        iSocketConnector = NULL;
        }
    if(iParser)
        {
        delete iParser;
        iParser = NULL;
        }
    if(iCommander)
        {
        delete iCommander;
        iCommander = NULL;
        }
    if(iListener)
        {
        delete iListener;
        iListener = NULL;
        }

#ifdef _SEND_KEYEVENTS
    iLastEvent.Set(TRawEvent::EKeyUp);
    UserSvr::AddEvent(iLastEvent);
#endif
    
#ifdef _USE_PS_ACC
    iProperty.Close();
    User::LeaveIfError(RProperty::Delete(KServerUid3,KAccPropertyX));
    User::LeaveIfError(RProperty::Delete(KServerUid3,KAccPropertyY));
    User::LeaveIfError(RProperty::Delete(KServerUid3,KAccPropertyZ));
#endif
    }

CConnectionData* CConnectionData::NewLC(MBTControlCallbacks& aObserver)
    {
    CConnectionData* self = new (ELeave) CConnectionData(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CConnectionData* CConnectionData::NewL(MBTControlCallbacks& aObserver)
    {
    CConnectionData* self = CConnectionData::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CConnectionData::ConstructL()
    {    
    iSocketConnector = CSocketConnector::NewL(*this);
    iParser = CCommandParser::NewL(*this);
    iCommander = CCommandCreator::NewL(*this);
    iListener = CForegroundListener::NewL(*this);     
    
#ifdef _USE_PS_ACC
    TInt err(0);
    err = RProperty::Define(KServerUid3,KAccPropertyX,RProperty::EInt,KAllowAllPolicy,KWriteUserDataPolicy,0);
    err = RProperty::Define(KServerUid3,KAccPropertyY,RProperty::EInt,KAllowAllPolicy,KWriteUserDataPolicy,0);
    err = RProperty::Define(KServerUid3,KAccPropertyZ,RProperty::EInt,KAllowAllPolicy,KWriteUserDataPolicy,0);
    err = iProperty.Attach(KServerUid3,KAccPropertyX,EOwnerThread);
    err = iProperty.Attach(KServerUid3,KAccPropertyY,EOwnerThread);
    err = iProperty.Attach(KServerUid3,KAccPropertyZ,EOwnerThread);
#endif
    }

TInt CConnectionData::AddConnectionL(TL2CAPSockAddr aAddr)
    {
    TInt ret(KErrGeneral);

    if(iRemoteInfo.iConnected)
        {
        ret = KErrAlreadyExists;
        }
    else
        {
        switch(iRemoteInfo.iType)
            {
            case TControllerDataTypes::ERemoteTypeWii:
                {
                iRemoteInfo.iNumber = (TUint8)TControllerDataTypes::EWiiMoteLed1;
                iRemoteInfo.iReportType = KWiiMoteContinuousReporting;                
                }break;
            default:
                break;
            }
        iRemoteInfo.iAddress = aAddr;    
        ret = KErrNone;
        }
    return ret;
    }

void CConnectionData::Disconnect()
    {
    iSocketConnector->Cancel();
    iParser->Cancel();
    iSocketConnector->Disconnect();

#ifdef _SEND_KEYEVENTS
    iLastEvent.Set(TRawEvent::EKeyUp);
    UserSvr::AddEvent(iLastEvent);
#endif
    }

void CConnectionData::ReConnect()
    {
    iState = EReconnect;
    Disconnect();
    }

void CConnectionData::ConnectL(TL2CAPSockAddr aAddr)
    {
    iSocketConnector->ConnectDeviceL(aAddr);
    }

void CConnectionData::Remove()
    {
    iRemoteInfo.iConnected = EFalse;
    }

TBool CConnectionData::ConnectionExists(TL2CAPSockAddr aAddr)
    {
    TBool ret(EFalse);
    if(iRemoteInfo.iAddress == aAddr)
        {
        ret = ETrue;
        }
    return ret;
    }

CSocketConnector& CConnectionData::Connection()
    {
    return *iSocketConnector;
    }

void CConnectionData::Connected(TL2CAPSockAddr aAddr)
    {
    iRemoteInfo.iConnected = ETrue;
    iRemoteInfo.iAddress = aAddr;
    
    iRemoteInfo.iWiiContinuousReport = 0;
    iRemoteInfo.iCommand = TControllerDataTypes::EWiiMoteCommandLED;
    iCommander->SendSettingWii(iRemoteInfo);            
    }

CCommandParser& CConnectionData::Parser()
    {
    return *iParser;
    }

CCommandCreator& CConnectionData::CommandCreator()
    {
    return *iCommander;
    }

TRemoteInfo& CConnectionData::RemoteInfo()
    {
    return iRemoteInfo;
    }

void CConnectionData::UpdateRemoteInfo(TRemoteInfo aInfo)
    {
	iRemoteInfo = aInfo;
    if(aInfo.iCommand)
    	{
        iObserver.SendUpdate(aInfo);
        }
    }

void CConnectionData::SendAccValues(TUint16 aAccX,TUint16  aAccY,TUint16  aAccZ)
    {    
#ifdef _USE_PS_ACC
    TInt err;
    err = iProperty.Set(KServerUid3,KAccPropertyX,aAccX);
    err = iProperty.Set(KServerUid3,KAccPropertyY,aAccY);
    err = iProperty.Set(KServerUid3,KAccPropertyZ,aAccZ);
#endif
    }

void CConnectionData::SendButtonClick(TStdScanCode aScanCode, TControllerDataTypes::TButtonClick aType)
    {
    TStdScanCode iScanCode;
    iScanCode = aScanCode;
    
    if(!iCanSend)
        {
        iScanCode = EStdKeyApplication0;
        }

#ifdef _SEND_KEYEVENTS
    TRawEvent event;

    switch(aType)
        {
        case TControllerDataTypes::EButtonClickRepeat:
            {
            event.Set(TRawEvent::EKeyRepeat,iScanCode);
            }break;
        case TControllerDataTypes::EButtonClickJustPressed:
        case TControllerDataTypes::EButtonClickPressed:
            {
            event.Set(TRawEvent::EKeyDown,iScanCode);
            }break;
        case TControllerDataTypes::EButtonClickReleased:
            {
            event.Set(TRawEvent::EKeyUp,iScanCode);
            }break;
        
        default:
            break;
        }
    iLastEvent = event;    
    UserSvr::AddEvent(event);
#endif
    }

void CConnectionData::CommandReady(const TDesC8& aData)
    {
    iSocketConnector->SendData(aData);
    }

void CConnectionData::KeyConfigReady(TInt aError)
    {
    iObserver.KeyConfigReady(aError);
    }

void CConnectionData::SocketConnectComplete(TInt aError, TL2CAPSockAddr aAddr)
    {
    (void)aError;
    TInt err(0);
        
    if( iSocketConnector->Connected() && !err)
        {
        if(!iSentConnectionStatus)
            {
            iObserver.ConnectDeviceComplete(KErrNone);
            iSentConnectionStatus = ETrue;
            Connected(aAddr);                        
            }        
        }
    else
        {
        if(!iSentConnectionStatus)
            {
            iObserver.ConnectDeviceComplete(KErrCouldNotConnect);
            iSentConnectionStatus = ETrue;
            Disconnect();
            }
        }    
    }

void CConnectionData::HandshakeComplete()
    {
    TRemoteInfo info;
    info = iRemoteInfo;
    info.iCommand = TControllerDataTypes::EWiiMoteCommandReportType;
    iCommander->SendSettingWii(info);            
    }

void CConnectionData::SocketDisconnectComplete(TInt aError)
    {
    (void)aError;    
    Remove();
    iSentConnectionStatus = EFalse;       
    if(iState == EReconnect)
        {
        iState = ENormal;
        ConnectL(iRemoteInfo.iAddress);        
        }
    if(aError == KErrDisconnected)
        {
        iObserver.DisconnectDeviceComplete(KErrDisconnected);
        }
    }

void CConnectionData::SocketReceiveComplete(const TDesC8& aData)
    {
    iParser->DataReadyL(aData);    
    }

void CConnectionData::SetForegroundSending(TBool aCanSend)
    {
    iCanSend = aCanSend;
    if(iRemoteInfo.iLoadConfigByUid)
        {
        TFileName fn;
        iListener->GetUidConfig(fn);
        if(fn != KNullDesC)
            iParser->LoadKeyConfig(fn);
        }    
    }
