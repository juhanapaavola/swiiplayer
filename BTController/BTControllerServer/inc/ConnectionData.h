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
#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <bt_sock.h>
#include <bttypes.h>
#include <e32keys.h>
#include <e32event.h>
#include <e32svr.h>
#ifdef _USE_PS_ACC
#include <e32property.h>
#endif
#include "btcontrollercommon.h"
#include "socketconnector.h"
#include "commandparser.h"
#include "mconnectiondatacallbacks.h"
#include "tremoteinfo.h"
#include "commandcreator.h"
#include "btcontroller.h"
#include "foregroundlistener.h"

class CSocketConnector;
class CBTController;

class CConnectionData : public CBase, MConnectionDataCallbacks
    {
public:
    ~CConnectionData();

    static CConnectionData* NewL(MBTControlCallbacks& aObserver);

    static CConnectionData* NewLC(MBTControlCallbacks& aObserver);

    TInt AddConnectionL(TL2CAPSockAddr aAddr);
    
    void Disconnect();
    
    void ReConnect();
    
    void ConnectL(TL2CAPSockAddr aAddr);
    
    void Remove();
    
    CSocketConnector& Connection();
    
    CCommandParser& Parser();
    
    CCommandCreator& CommandCreator();
    
    TRemoteInfo& RemoteInfo();
    
    void UpdateRemoteInfo(TRemoteInfo aInfo);
    
    TBool ConnectionExists(TL2CAPSockAddr aAddr);
    
    void UpdatePS3(TRemoteInfo& aInfo);

    void Connected(TL2CAPSockAddr aAddr);
    
protected:
    
    void SendAccValues(TUint16 aAccX,TUint16  aAccY,TUint16  aAccZ);
    
    void SendButtonClick(TStdScanCode aScanCode, TControllerDataTypes::TButtonClick aType);
    
    void CommandReady(const TDesC8& aData);
    
    void KeyConfigReady(TInt aError);
    
    void SocketConnectComplete(TInt aError, TL2CAPSockAddr aAddr);
    
    void SocketDisconnectComplete(TInt aError);
    
    void SocketReceiveComplete(const TDesC8& aData);
    
    void HandshakeComplete();

    void SetForegroundSending(TBool aCanSend);
        
    
private:

    CConnectionData(MBTControlCallbacks& aObserver);

    void ConstructL();
    
    TBool GetForegroundApp();
    
private:
    enum {
        ENormal = 0,
        EDisconnect,
        EConnect,
        EReconnect     
    };
    
    MBTControlCallbacks&	iObserver;
    TInt                	iState;
    TL2CAPSockAddr      	iAddr;
    CSocketConnector*   	iSocketConnector;
    CCommandParser*     	iParser;
    CCommandCreator*    	iCommander;
    CForegroundListener* 	iListener;
#ifdef _SEND_KEYEVENTS
    TRawEvent           	iLastEvent;
#endif
    TRemoteInfo         	iRemoteInfo;
    TRawEvent           	iKeyEvent;
#ifdef _USE_PS_ACC
    RProperty           	iProperty;
#endif
    TBool               	iSentConnectionStatus;
    TBool               	iCanSend;
    };

#endif

