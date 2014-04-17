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
#ifndef MCONNECTIONDATACALLBACKS_H_
#define MCONNECTIONDATACALLBACKS_H_

#include <e32keys.h>
#include <TControllerDataTypes.h>
#include "tremoteinfo.h"

class MConnectionDataCallbacks
    {
public:
    
    virtual void SendButtonClick(TStdScanCode aScanCode, TControllerDataTypes::TButtonClick aType) = 0;
    
    virtual void CommandReady(const TDesC8& aData) = 0;
    
    virtual void KeyConfigReady(TInt aError) = 0;
    
    virtual TRemoteInfo& RemoteInfo() = 0;
    
    virtual void UpdateRemoteInfo(TRemoteInfo aInfo) = 0;
    
    virtual void SendAccValues(TUint16 aAccX,TUint16  aAccY,TUint16  aAccZ) = 0;
    
    // socket
    virtual void SocketConnectComplete(TInt aError, TL2CAPSockAddr aAddr) = 0;
    
    virtual void SocketDisconnectComplete(TInt aError) = 0;
    
    virtual void SocketReceiveComplete(const TDesC8& aData) = 0;
    
    virtual void HandshakeComplete() = 0;

    virtual void SetForegroundSending(TBool aCanSend) = 0;
        
    };

#endif

