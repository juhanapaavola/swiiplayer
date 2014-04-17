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
#ifndef SOCKETCONNECTOR_H_
#define SOCKETCONNECTOR_H_

#include <e32base.h>
#include <e32std.h>
#include <bt_sock.h>
#include <bttypes.h>
#include "MConnectionDataCallbacks.h"
#include "btcontrollercommon.h"
#include <TControllerDataTypes.h>

const TL2CAPPort KOutputChannel = 0x0011;
const TL2CAPPort KInputChannel = 0x0013;

class CSocketConnector : public CActive
    {
public:
    ~CSocketConnector();

    static CSocketConnector* NewL(MConnectionDataCallbacks& aObserver);

    static CSocketConnector* NewLC(MConnectionDataCallbacks& aObserver);

    void ConnectDeviceL(TL2CAPSockAddr aBtAddr);
    
    void Disconnect(TInt aError = KErrNone);
    
    void SendData(const TDesC8& aData);
    
    TBool Connected();
    
private:
    CSocketConnector(MConnectionDataCallbacks& aObserver);

    void ConstructL();

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

    void Receive();

    TL2CAPSockAddr& BtAddress();

    void ConnectInterrupt();
    
    void HandShake(const TDesC8& aData);
    
    void WaitHandshake();
    
    void HandshakeHID();
    
    void WaitHandshakeHID();
    
private:
    enum TConnector2State
        {
        EUninitialized,
        EInitialized,
        EError,
        EConnecting,
        EConnectingInterrupt,
        EConnected,
        EReceiving,
        ESending,
        EHandshakeHID,
        EHandshakeHIDFinish,
        EHandshakeWii,
        EHandshakeWiiFinish
        };
            
private:

    MConnectionDataCallbacks&       iObserver;
    RSocketServ                     iSS;
    RSocket                         iIntSocket;
    RSocket                         iCtlSocket;
    TSockXfrLength                  iDataLen;
    TBuf8<255>                      iBuf;
    TUint8*                         iDataRecv;        
    TL2CAPSockAddr                  iBtAddr;
    TInt                            iLastError;
    TBool                           iConnected;
    TInt                            iControllerNumber;
    TInt                            iState;
    TPtr8                           iBufferPtr;
    };

#endif

