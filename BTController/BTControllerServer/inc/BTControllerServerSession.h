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
#ifndef BTCONTROLLERSERVERSESSION_H
#define BTCONTROLLERSERVERSESSION_H

#include <e32std.h>
#include <e32base.h>
#include "mbtcontrollerserversessioncallbacks.h"
#include "btcontroller.h"
#include "btcontrollerserver.h"

class CBTControllerServerSession : public CSession2, MBTControllerServerSessionCallbacks
    {
public:
    ~CBTControllerServerSession();

    static CBTControllerServerSession* NewL(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer);

    static CBTControllerServerSession* NewLC(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer);

    void ServiceL( const RMessage2& aMessage );
   
protected:
    
    void Powered(TInt aPowerStatus);
    
    void ShutdownComplete();
    
    void ControllerDiscoverComplete(TInt aStatus, TBTDevAddr aAddr);
    
    void ControllerStartComplete(TInt aError);
    
    void ConnectToComplete(TInt aStatus);
    
    void KeyConfigLoadComplete(TInt aError);
    
    void GetBatteryStatusComplete(TInt aBatteryStatus, TInt aError);
    
    void GetSpeakerEnabledComplete(TControllerDataTypes::TWiiSpeaker aEnabled, TInt aError);
    
    void SetWiiSpeakerStatusComplete(TControllerDataTypes::TWiiSpeaker aState, TInt aError);
        
    void ErrorComplete(TInt aError);
    
private:

    CBTControllerServerSession(CObjectConIx& aSessionContainer,CBTController& aController,CBTControllerServer *aServer);

    void ConstructL();

    CBTControllerServer& Server();

    void PanicClient(const RMessagePtr2& aMessage, TInt aError);
    
    void ControllerStart();
    
    void ControllerStop();
    
    void ControllerDiscover(TControllerDataTypes::TRemoteType aType);
    
    void SwitchOnPower();    
        
    void LoadKeyConfig(const TDesC& aFilename);
    
    void GetControllerType();
    
    void GetBatteryStatus();
    
    void GetWiiSpeakerStatus();
    
    void SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aState);
    
    void EnableWiiAcc(TBool aState);
        
    void SetLoadByUid(TBool aLoadByUid);
    
    void GetCurrentConfigName();
    
private:
    CObjectConIx&           iSessionContainer;
    CBTController&          iController;
    CBTControllerServer*    iServer;    
    RMessage2               iMessage;
    TBool                   iControllerStarted;
    TServerMessage 			iLastFunction;
    TFileName				iKeyConfigName;
    };

#endif
