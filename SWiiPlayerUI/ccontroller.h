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

#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include "controllerwrapper.h"
#include <RBTControllerClient.h>
#include <bt_sock.h>
#include <bttypes.h>
#include <TControllerDataTypes.h>

class CController: CActive
{
public:
    static CController* NewL(ControllerWrapper &aPublicAPI);

    ~CController();

public:
    void getBatteryStatus();
    void getRemoteType();
    void disconnectRemote();
    void Power();
    void LoadConfig(const TDesC& aName);
    void LoadConfigByUid(TBool aLoadByUid);
    void GetCurrentConfigName(TFileName& aFilename);
    void findAndConnect();

public: //From CActive
    virtual void DoCancel();
    void RunL();

private:
    CController(ControllerWrapper &aPublicAPI);
    void ConstructL();
                
private: // Data
    enum {
        ENothing = 0,
        EFindAndConnect,
        EDisconnect,
        EBatteryStatus,
        ERemoteType,
        EPower,
        ELoadConfig
    };

    ControllerWrapper&  iPublicAPI;
    TInt                iState;    
    RBTControllerClient 		iClient;
    TL2CAPSockAddr      iAddr;
    TInt                iBattery;
    TControllerDataTypes::TRemoteType         iType;
    TBool               iConnected;
    HBufC*              iFilename;
};

#endif // CCONTROLLER_H
