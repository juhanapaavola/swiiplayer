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
#include "socketconnector.h"

CSocketConnector::CSocketConnector(MConnectionDataCallbacks& aObserver) :
    CActive(EPriorityStandard),
            iObserver(aObserver),
                    iBufferPtr(0,0)
    {
    }

CSocketConnector* CSocketConnector::NewLC(MConnectionDataCallbacks& aObserver)
    {
    CSocketConnector* self = new (ELeave) CSocketConnector(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CSocketConnector* CSocketConnector::NewL(MConnectionDataCallbacks& aObserver)
    {
    CSocketConnector* self = CSocketConnector::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CSocketConnector::ConstructL()
    {
    CActiveScheduler::Add(this);
    User::LeaveIfError(iSS.Connect());
    iLastError = KErrNone;
    iConnected = EFalse;
    iDataRecv = (TUint8*)User::AllocL(MAX_DATA_SIZE);
    iBufferPtr.Set(iDataRecv,MAX_DATA_SIZE,MAX_DATA_SIZE);
    }

CSocketConnector::~CSocketConnector()
    {
    Cancel();
    iIntSocket.Close();
    iCtlSocket.Close();
    iSS.Close();

    if(iDataRecv)
        {
        User::Free(iDataRecv);
        iDataRecv = NULL;
        }
    }

void CSocketConnector::ConnectDeviceL(TL2CAPSockAddr aBtAddr)
    {
    TInt err;
    iBtAddr = aBtAddr;
    iBtAddr.SetPort(KOutputChannel);                                 
       
    err = iCtlSocket.Open(iSS,KL2CAPDesC());
    iCtlSocket.Connect(iBtAddr,iStatus);

    iState = EConnecting;
    SetActive();
    }

void CSocketConnector::RunL()
    {   
    switch(iState)
        {
        case EConnecting:
            {
            TInt err(0);
            err = iStatus.Int();
            if(err == KErrNone)
                {
                ConnectInterrupt();
                }
            else
                {
                iCtlSocket.Close();
                iObserver.SocketConnectComplete(err, iBtAddr );
                }
            }break;
        case EConnectingInterrupt:
            {
            TInt err(0);
            err = iStatus.Int();
            iLastError = err;
            
            if(err == KErrNone)
                {                
                iConnected = ETrue;
                HandshakeHID();
                }            
            else
                {
                iCtlSocket.Close();
                iIntSocket.Close();
                }
            
            }break;
        case EHandshakeHID:
            {
            WaitHandshakeHID();
            }break;
        case EHandshakeHIDFinish:
            {
            iState = EConnected;
            iObserver.SocketConnectComplete(iLastError, iBtAddr );            
            }break;
        case ESending:
            {            
            Receive();
            }break;
        case EReceiving:
            {
            TInt err(0);
            err = iStatus.Int();
            if(err == KErrNone)
                {
                iObserver.SocketReceiveComplete(iBufferPtr);            
                Receive();
                }
            else
                {
                Disconnect(KErrDisconnected);
                }
            }break;
        case EHandshakeWii:
            {
            iObserver.HandshakeComplete();
            Receive();

            }break;
        default:
            break;
        }    
    }

TInt CSocketConnector::RunError(TInt aError)
    {
    return aError;
    }

void CSocketConnector::ConnectInterrupt()
    {
    TInt err(0);
    iBtAddr.SetPort(KInputChannel);
    err = iIntSocket.Open(iSS,KL2CAPDesC());
    iIntSocket.Connect(iBtAddr,iStatus);
    
    iState = EConnectingInterrupt;
    SetActive();
    }

void CSocketConnector::Receive()
    {
    iBufferPtr.Zero();
    iIntSocket.Read(iBufferPtr,iStatus);
    iState = EReceiving;
    SetActive();
    }

void CSocketConnector::SendData(const TDesC8& aData)
    {
    switch(iState)
        {
        case EConnected:
            {
            HandShake(aData);
            }break;
        case EReceiving:
            {
            Cancel();
            iState = ESending;
            iCtlSocket.Write(aData,iStatus);    
            SetActive();            
            }break;
        default:
            break;
        }
    }

void CSocketConnector::DoCancel()
    {
    iIntSocket.CancelAll();
    iCtlSocket.CancelAll();
    }

TBool CSocketConnector::Connected()
    {
    return iConnected;
    }

void CSocketConnector::Disconnect(TInt aError)
    {
    Cancel();
    iIntSocket.Close();
    iCtlSocket.Close();
    iObserver.SocketDisconnectComplete(aError);
    }

void CSocketConnector::HandShake(const TDesC8& aData)
    {
    if(iState != EConnected)
        return;
        
    iState = EHandshakeWii;

    Cancel();
    iCtlSocket.Write(aData,iStatus);    
    SetActive();    
    }

void CSocketConnector::WaitHandshake()
    {
    iBufferPtr.Zero();
    iIntSocket.Read(iBufferPtr,iStatus);
    iState = EHandshakeWiiFinish;
    SetActive();
    }

void CSocketConnector::WaitHandshakeHID()
    {
    iBufferPtr.Zero();
    iIntSocket.Read(iBufferPtr,iStatus);
    iState = EHandshakeHIDFinish;
    SetActive();    
    }

void CSocketConnector::HandshakeHID()
    {
    iState = EHandshakeHID;
    TUint8* data;
    TInt len(0);
    
    len = 3;
    data = (TUint8*)User::AllocLC(len);
    data[0] = (TUint8)TControllerDataTypes::EWiiMoteOutput|KWiiMoteSetReport;
    data[1] = (TUint8)TControllerDataTypes::EWiiMoteCommandLED;
    data[2] = 0x00;

    TPtr8 ptr(data,len,len);
    iCtlSocket.Write(ptr,iStatus);    
    CleanupStack::PopAndDestroy(data);
    SetActive();
    }
