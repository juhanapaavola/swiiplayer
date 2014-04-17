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
#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include <e32base.h>
#include <e32std.h>
#include "mbtcontrolcallbacks.h"
#include "MBTControllerServerSessionCallbacks.h"
#include "devicefinder.h"
#include "btpowerquery.h"
#include "socketconnector.h"
#include "btcontrollercommon.h"
#include "connectiondata.h"

class CConnectionData;

class CBTController : public MBTControlCallbacks
    {
public:
    virtual ~CBTController();

    static CBTController* NewL();

    static CBTController* NewLC();

    void StartL(MBTControllerServerSessionCallbacks* aServerSession);
    
    void Stop();
    
    void StartDiscoverL();
    
    void PowerOn();
    
    void SetSocketSecurity(TBTServiceSecurity aSecurity, TInt aType);
    
    void ConnectDevice(TL2CAPSockAddr aAddr);
    
    void DisconnectDevice();
    
    void LoadKeyConfig(const TDesC& aFilename);
    
    void GetControllerType(TControllerDataTypes::TRemoteType& aType);
    
    TInt GetBatteryStatus();
    
    TInt GetWiiSpeakerStatus();
    
    TInt SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aState);
    
    TInt EnableWiiAcc(TBool aEnable);
    
    void CancelDiscovery();
    
    void SetLoadByUid(TBool aLoadByUid);
	
    void GetConfigName(TFileName& aFilename);
    
protected:
    void DeviceSelected(TBTDevAddr aAddr, TControllerDataTypes::TRemoteType aType, TInt aError);
    
    void ServiceSearchFinished(TInt aError);
    
    void ServiceFound(TBool aFound);

    void PowerState(TBool aPower);

    void SocketConnectComplete(TInt aError, TL2CAPSockAddr aAddr);

    void SocketDisconnectComplete(TInt aError);

    void SocketReceiveComplete(const TDesC8& aData);

    void HandshakeComplete();
    
    void KeyConfigReady(TInt aError);
    
    void SendUpdate(TRemoteInfo aInfo);
    
    void ConnectDeviceComplete(TInt aError);
    
    void DisconnectDeviceComplete(TInt aError = 0);
    
private:
    CBTController();

    void ConstructL();

private:

    enum TControllerState {
        EControllerStopped,
        EControllerStarting,
        EControllerStarted        
    };
    
private:

    MBTControllerServerSessionCallbacks*    iServerSession;
    CBTPowerQuery*                          iPowerQuery;
    CDeviceFinder*                          iDeviceFinder;
    CConnectionData*                        iConnections;

    TInt                                    iState;
    TBTDevAddr                              iAddr;
    TBool                                   iPower;
    TL2CAPSockAddr                          iControlAddr;
    TL2CAPSockAddr                          iInterruptAddr;
    TBTServiceSecurity                      iControlSecurity;
    TBTServiceSecurity                      iInterruptSecurity;
    TInt                                    iControllerAmount;
    TBool                                   iUserAskedPower;
    TBool                                   iReDiscover;
    };

#endif

