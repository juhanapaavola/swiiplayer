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
#include "BTControllerServerSession.h"

CBTControllerServerSession::CBTControllerServerSession(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer):
iSessionContainer(aSessionContainer),
iController(aController),
iServer(aServer),
iControllerStarted(EFalse),
iLastFunction(ELastMessage)
    {
    }

CBTControllerServerSession::~CBTControllerServerSession()
    {
    iServer->DropSession();
    iControllerStarted = EFalse;
    }

CBTControllerServerSession* CBTControllerServerSession::NewLC(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer)
    {
    CBTControllerServerSession* self = new (ELeave) CBTControllerServerSession(aSessionContainer,aController,aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CBTControllerServerSession* CBTControllerServerSession::NewL(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer)
    {
    CBTControllerServerSession* self = CBTControllerServerSession::NewLC(aSessionContainer,aController,aServer);
    CleanupStack::Pop();
    return self;
    }

void CBTControllerServerSession::ConstructL()
    {
    iServer->AddSession();
    }

CBTControllerServer& CBTControllerServerSession::Server()
    {
    return *static_cast<CBTControllerServer*> ( const_cast<CServer2*>( CSession2::Server() ) );
    }

void CBTControllerServerSession::ServiceL( const RMessage2& aMessage )
    {
    RDebug::Print(_L("CBTControllerServerSession::ServiceL: %d"), aMessage.Function());
    if(aMessage.Function() != ECancelAll)
        iLastFunction = (TServerMessage)aMessage.Function();
    
    switch(aMessage.Function())
        {
        case EStartServer:
            {
            iMessage = aMessage;
            ControllerStart();
            }break;
            
        case EStopServer:
            {
            iMessage = aMessage;
            ControllerStop();
            }break;
            
        case EFindAndConnect:
        	{
        	iMessage = aMessage;
            ControllerDiscover(TControllerDataTypes::ERemoteTypeWii);
        	}break;

        case EDisconnect:
        	{
        	iMessage = aMessage;
        	iController.DisconnectDevice();
        	iMessage.Complete(KErrNone);
        	}break;
        	
        case ESwitchPowerOn:
            {
            iMessage = aMessage;
            SwitchOnPower();
            }break;

        case ELoadKeyConfig:
            {
            iMessage = aMessage;
            iKeyConfigName.Zero();
            iMessage.ReadL(0,iKeyConfigName);
            LoadKeyConfig(iKeyConfigName);
            }break;
            
        case EGetCurrentConfig:
            {
            iMessage = aMessage;
            GetCurrentConfigName();
            }break;
            
        case EControllerType:
            {
            iMessage = aMessage;
            GetControllerType();
            }break;
            
        case EBatteryStatus:
            {
            iMessage = aMessage;
            GetBatteryStatus();
            }break;
            
        case EWiiSpeakerEnabled:
            {
            iMessage = aMessage;
            GetWiiSpeakerStatus();            
            }break;
            
        case ESetWiiSpeakerEnabled:
            {
            iMessage = aMessage;
            TControllerDataTypes::TWiiSpeaker spk;
            TPtr8 desc(reinterpret_cast<TUint8*>(&spk),sizeof(spk),sizeof(spk));
            iMessage.ReadL(0,desc);
            SetWiiSpeakerStatus(spk);
            }break;
            
        case ESetAccelerometerState:
            {
            iMessage = aMessage;
            TBool state;
            TPtr8 desc(reinterpret_cast<TUint8*>(&state),sizeof(state),sizeof(state));
            iMessage.ReadL(0,desc);
            EnableWiiAcc(state);            
            }break;

        case ECancelAll:
            {
            switch(iLastFunction)
                {
                case EDiscoverController:
                    {
                    iController.CancelDiscovery();
                    }break;
                default:
                    break;
                }
            if(!iMessage.IsNull())
                {
                iMessage.Complete(KErrCancel);
                }
            if(!aMessage.IsNull())
                {                      
					if(aMessage != iMessage)
						aMessage.Complete(KErrCancel);
                }
            iLastFunction = ECancelAll;
            }break;
            
        case ELoadConfigByUid:
            {
            iMessage = aMessage;
            TBool load;
            TPtr8 desc(reinterpret_cast<TUint8*>(&load),sizeof(load),sizeof(load));
            iMessage.ReadL(0,desc);
            SetLoadByUid(load);
            }break;
            
        default:
        	{
        	iMessage = aMessage;
        	PanicClient(iMessage,KErrNotSupported);            
        	}break;
        }    
    }

void CBTControllerServerSession::PanicClient(const RMessagePtr2& aMessage, TInt aError)
	{
	aMessage.Panic(_L("BTControllerServerSession"),aError);
	}

void CBTControllerServerSession::ControllerStart()
    {    
    if(iControllerStarted)
        {
        iMessage.Complete(KErrAlreadyExists);
        }
    else
        {
        iController.StartL(this);
        iControllerStarted = ETrue;
        if(!iMessage.IsNull())
            {
            iMessage.Complete(KErrNone);
            }
        }
    }

void CBTControllerServerSession::ControllerStop()
    {
    iController.Stop();
    if(!iMessage.IsNull())
        {
        iMessage.Complete(KErrNone);
        }
    }

void CBTControllerServerSession::SwitchOnPower()
    {
    iController.PowerOn();
    }

void CBTControllerServerSession::Powered(TInt aPowerStatus)
    {
    if(iMessage.Function() == ESwitchPowerOn)
        {
        if(!iMessage.IsNull())
            {
            iMessage.Complete(aPowerStatus);
            }
        }
    }

void CBTControllerServerSession::ShutdownComplete()
    {
    iControllerStarted = EFalse;
    }

void CBTControllerServerSession::ControllerDiscover(TControllerDataTypes::TRemoteType aType)
    {
    switch(aType)
        {
        case TControllerDataTypes::ERemoteTypeWii:
            {
            iController.StartDiscoverL();
            }break;
        
        default:
            iMessage.Complete(KErrNotSupported);
            break;
        }
    }

void CBTControllerServerSession::ControllerDiscoverComplete(TInt aStatus, TBTDevAddr aAddr)
    {    
    if(aStatus == KErrNone)
    	{
    	TL2CAPSockAddr a;
    	a.SetBTAddr(aAddr);
    	iController.ConnectDevice(a);
    	}
    else
    	{
    	if(!iMessage.IsNull())
    		iMessage.Complete(aStatus);
    	}
    }

void CBTControllerServerSession::ConnectToComplete(TInt aStatus)
    {
    if(EFindAndConnect == iMessage.Function())
    	{
    	TInt err(KErrNone);
    	if(aStatus == KErrNone)
    		err = iController.GetBatteryStatus();
    	
    	if(!iMessage.IsNull())
    		iMessage.Complete(aStatus);
    	}
    }

void CBTControllerServerSession::LoadKeyConfig(const TDesC& aFilename)
    {
    iController.LoadKeyConfig(aFilename);    
    }

void CBTControllerServerSession::KeyConfigLoadComplete(TInt aError)
    {
    if(!iMessage.IsNull())
        {
        iMessage.Complete(aError);
        }
    }

void CBTControllerServerSession::GetControllerType()
    {
    TControllerDataTypes::TRemoteType type;
    TPckg<TControllerDataTypes::TRemoteType> pckg( type );
    
    iController.GetControllerType(type);
    if(!iMessage.IsNull())
        {
        iMessage.Write(0,pckg);
        iMessage.Complete(KErrNone);
        }
    }

void CBTControllerServerSession::GetBatteryStatus()
    {
    TInt err(0);
    err = iController.GetBatteryStatus();
    }

void CBTControllerServerSession::GetBatteryStatusComplete(TInt aBatteryStatus, TInt aError)
    {
	if(iMessage.Function() == EBatteryStatus)
		{
        TPckg<TInt> pck(aBatteryStatus);
        if(!iMessage.IsNull())
            {
            TInt err(KErrNone);
            TInt bat(aBatteryStatus);
            TPtr8 ptr( reinterpret_cast<TUint8*>( &bat ), sizeof( bat ), sizeof( bat ) );
            err = iMessage.Write(0,ptr,0);
            iMessage.Complete(aError);
            }			
		}	
    }

void CBTControllerServerSession::GetSpeakerEnabledComplete(TControllerDataTypes::TWiiSpeaker aEnabled, TInt aError)
    {
    if(iMessage.Function() == EWiiSpeakerEnabled)
        {
        TPckg<TBool> pck(aEnabled);
        if(!iMessage.IsNull())
            {
            iMessage.Write(0,pck);
            iMessage.Complete(aError);
            }
        }
    }

void CBTControllerServerSession::GetWiiSpeakerStatus()
    {
    TInt err(0);
    err = iController.GetWiiSpeakerStatus();
    if(err != KErrNone)
        {
        TInt i(0);
        TPckg<TBool> pck(i);
        if(!iMessage.IsNull())
            {
            iMessage.Write(0,pck);
            iMessage.Complete(err);
            }
        }
    }

void CBTControllerServerSession::SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aState)
    {
    TInt err(0);
    err = iController.SetWiiSpeakerStatus(aState);
    if(err != KErrNone)
        {
        TInt i(0);
        TPckg<TBool> pck(i);
        if(!iMessage.IsNull())
            {
            iMessage.Write(0,pck);
            iMessage.Complete(err);
            }
        }
    }

void CBTControllerServerSession::SetWiiSpeakerStatusComplete(TControllerDataTypes::TWiiSpeaker aState, TInt aError)
    {
    if(iMessage.Function() == TControllerDataTypes::EWiiSpeakerEnabled)
        {
        TPckg<TControllerDataTypes::TWiiSpeaker> pck(aState);
        if(!iMessage.IsNull())
            {
            iMessage.Write(0,pck);        
            iMessage.Complete(aError);
            }
        }
    }

void CBTControllerServerSession::EnableWiiAcc(TBool aState)
    {
    TInt err(0);
    err = iController.EnableWiiAcc(aState);
    if(!iMessage.IsNull())
        iMessage.Complete(err);        
    }

void CBTControllerServerSession::ErrorComplete(TInt aError)
    {
    if(!iMessage.IsNull())
        {
        iMessage.Complete(aError);
        }
    }

void CBTControllerServerSession::SetLoadByUid(TBool aLoadByUid)
    {
    iController.SetLoadByUid(aLoadByUid);
    if(!iMessage.IsNull())
        iMessage.Complete(KErrNone);
    }
	
void CBTControllerServerSession::GetCurrentConfigName()
    {
    TFileName fn;
    iController.GetConfigName(fn);
    TPckg<TFileName> pck(fn);
    TInt err(KErrNone);
    err = iMessage.Write(0,pck);
    iMessage.Complete(err);
    }
    
