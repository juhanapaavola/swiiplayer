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
#include "RBTControllerClient.h"
#include <BTControllerCommon.h>

static TInt StartServer()
    {
    TInt err(0);
    TFindServer server(KServerName);
    TFullName name;
    const TUidType serverUid(KNullUid,KNullUid,KServerUid3);
    
    err = server.Next( name );
    if ( err != KErrNone )
        {
        RProcess sp;
        err = sp.Create( KServerNameExe, KNullDesC, serverUid ); 
    
        if (err==KErrNone)
            {
            TRequestStatus status;
            sp.Rendezvous(status);
    
            if (status != KRequestPending)
                {
                sp.Kill(0);
                }
            else
                {
                sp.Resume();
                }
        
            User::WaitForRequest(status);       
    
            err = (sp.ExitType() == EExitPanic) ? KErrGeneral : status.Int();
            sp.Close();                       
            }
        }
    return err;
    }

EXPORT_C RBTControllerClient::RBTControllerClient(): RSessionBase()
    {
    }

EXPORT_C RBTControllerClient::~RBTControllerClient()
    {
    }

EXPORT_C TInt RBTControllerClient::Connect()
    {
    TInt err(0);
    err = ::StartServer();
    
    if(err == KErrNone)
        {
        err = CreateSession(KServerName, Version());
        
        if(err == KErrNone)
            {
            err = SendReceive(EStartServer);
            }
        }
    return err;
    }

EXPORT_C void RBTControllerClient::FindAndConnect(TRequestStatus& aStatus)
	{
	SendReceive(EFindAndConnect,aStatus);
	}

EXPORT_C void RBTControllerClient::Disconnect()
	{
	SendReceive(EDisconnect);	
	}

EXPORT_C void RBTControllerClient::Close()
    {
    SendReceive(EStopServer);
    RSessionBase::Close();
    }

EXPORT_C TVersion RBTControllerClient::Version() const
	{
	return TVersion(KServerMajorVersionNumber, KServerMinorVersionNumber, KServerBuildVersionNumber);            
	}

EXPORT_C void RBTControllerClient::Power(TRequestStatus& aStatus )
    {
    TIpcArgs args; 
    SendReceive(ESwitchPowerOn,args, aStatus);
    }

EXPORT_C void RBTControllerClient::LoadKeyConfig(const TDes& aFilename, TRequestStatus& aStatus)
    {
	iFilename.Zero();
	iFilename.Copy(aFilename);
    TIpcArgs args(&iFilename);
    SendReceive(ELoadKeyConfig,args,aStatus);
    }

EXPORT_C void RBTControllerClient::ControllerType(TControllerDataTypes::TRemoteType& aType, TRequestStatus& aStatus)
    {
    TPckg<TControllerDataTypes::TRemoteType> pck(aType);
    TIpcArgs args(&pck);
    SendReceive(EControllerType,args,aStatus);
    }

EXPORT_C void RBTControllerClient::BatteryStatus(TInt& aBatteryStatus, TRequestStatus& aStatus)
    {
	TPtr8 descriptor( reinterpret_cast<TUint8*>( &iBatteryStatus ), sizeof( iBatteryStatus ), sizeof( iBatteryStatus ) );
    TIpcArgs args(&descriptor);
    SendReceive(EBatteryStatus,args,aStatus);
    aBatteryStatus = iBatteryStatus;
    }

EXPORT_C void RBTControllerClient::WiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker& aSpeakerStatus, TRequestStatus& aStatus)
    {
    TPckg<TControllerDataTypes::TWiiSpeaker> pck(aSpeakerStatus);
    TIpcArgs args(&pck);
    SendReceive(EWiiSpeakerEnabled,args,aStatus);        
    }

EXPORT_C void RBTControllerClient::SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aSpeakerStatus, TRequestStatus& aStatus)
    {
    TPckg<TControllerDataTypes::TWiiSpeaker> pck(aSpeakerStatus);
    TIpcArgs args(&pck);
    SendReceive(ESetWiiSpeakerEnabled,args,aStatus);        
    }

EXPORT_C void RBTControllerClient::SetWiiAccelerometerState(TBool aState, TRequestStatus& aStatus)
    {
    TPckg<TBool> pck(aState);
    TIpcArgs args(&pck);
    SendReceive(ESetAccelerometerState,args,aStatus);        
    }

/*
 * sync calls
 * 
 */

EXPORT_C void RBTControllerClient::Power()
    {
    TIpcArgs args; 
    SendReceive(ESwitchPowerOn,args);
    }

EXPORT_C void RBTControllerClient::LoadKeyConfig(const TFileName& aFilename)
    {
    TPckg<TFileName> pck(aFilename);
    TIpcArgs args(&pck);
    SendReceive(ELoadKeyConfig,args);
    }

EXPORT_C void RBTControllerClient::ControllerType(TControllerDataTypes::TRemoteType& aType)
    {
    TPckg<TControllerDataTypes::TRemoteType> pck(aType);
    TIpcArgs args(&pck);
    SendReceive(EControllerType,args);
    }

EXPORT_C void RBTControllerClient::BatteryStatus(TInt& aBatteryStatus)
    {
    TPckg<TInt> pck(aBatteryStatus);
    TIpcArgs args(&pck);
    SendReceive(EBatteryStatus,args);    
    }

EXPORT_C void RBTControllerClient::WiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker& aSpeakerStatus)
    {
    TPckg<TControllerDataTypes::TWiiSpeaker> pck(aSpeakerStatus);
    TIpcArgs args(&pck);
    SendReceive(EWiiSpeakerEnabled,args);        
    }

EXPORT_C void RBTControllerClient::SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aSpeakerStatus)
    {    
    TPckg<TControllerDataTypes::TWiiSpeaker> pck(aSpeakerStatus);
    TIpcArgs args(&pck);
    SendReceive(ESetWiiSpeakerEnabled,args);        
    }

EXPORT_C void RBTControllerClient::SetWiiAccelerometerState(TBool aState)
    {    
	TPckg<TBool> pck(aState);
    TIpcArgs args(&pck);
    SendReceive(ESetAccelerometerState,args);        
    }

EXPORT_C void RBTControllerClient::SetLoadConfigByUid(TBool aLoadByUid)
    {
	TPckg<TBool> pck(aLoadByUid);
    TIpcArgs args(&pck);
    SendReceive(ELoadConfigByUid,args);
    }

EXPORT_C void RBTControllerClient::CancelAll()
    {
    TIpcArgs args;
    SendReceive(ECancelAll,args);
    }

EXPORT_C void RBTControllerClient::GetCurrentConfigName(TFileName& aFilename)
    {
    TPckg<TFileName> pck(aFilename);
    TIpcArgs args(&pck);
    SendReceive(EGetCurrentConfig,args);
    }
