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
#include "DeviceFinder.h"

CDeviceFinder::CDeviceFinder(MBTControlCallbacks& aObserver) :
    CActive(EPriorityStandard),
    iState(EUninitialized),
    iObserver(aObserver),
    iConnected(EFalse)
    {
    }

CDeviceFinder* CDeviceFinder::NewLC(MBTControlCallbacks& aObserver)
    {
    CDeviceFinder* self = new (ELeave) CDeviceFinder(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CDeviceFinder* CDeviceFinder::NewL(MBTControlCallbacks& aObserver)
    {
    CDeviceFinder* self = CDeviceFinder::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CDeviceFinder::ConstructL()
    {
    CActiveScheduler::Add(this);
    iConnected = EFalse;
    User::LeaveIfError(iSockServ.Connect());
    }

CDeviceFinder::~CDeviceFinder()
    {
    Cancel();
    if(iConnected)
        {
        iNotifier.Close();
        }
    iSockServ.Close();
    iResolver.Close();
    }

void CDeviceFinder::DoCancel()
    {
    iResolver.Cancel();
    iResolver.Close();
    if(iConnected)
        {
        iNotifier.CancelNotifier(KDeviceSelectionNotifierUid);
        }
    }


void CDeviceFinder::RunL()
    {
    switch(iState)
        {
        case EDiscoverNoUi:
            {
            TInt err(0);
            err = iStatus.Int();
            TBTDevAddr addr;
            TInquirySockAddr& sa = TInquirySockAddr::Cast( iEntry().iAddr );
            TBTDeviceClass tmpDeviceClass( sa.MajorServiceClass(), sa.MajorClassOfDevice(), sa.MinorClassOfDevice());
            addr = static_cast<TBTSockAddr>(iEntry().iAddr).BTAddr();
            
            if(iStatus.Int() == KErrNone)
                {
                if(tmpDeviceClass.DeviceClass() == KWiiMoteDeviceClass) 
                    {
                    iObserver.DeviceSelected(addr,TControllerDataTypes::ERemoteTypeWii,KErrNone);
                    iResolver.Close();
                    iSockServ.Close();
                    }
                else
                    {
                    ContinueDiscover();
                    }
                }
            else
                {
                iObserver.DeviceSelected(addr,TControllerDataTypes::ERemoteTypeNoController,KErrNotFound);
                iResolver.Close();
                iSockServ.Close();
                }          
            }break;
            
        case EDiscoverWithUi:
            {
            TBTDeviceResponseParams resp = iResParams();                        
            iConnected = EFalse;
            iNotifier.CancelNotifier(KDeviceSelectionNotifierUid);
            iNotifier.Close();

            TInquirySockAddr& sa = TInquirySockAddr::Cast( iEntry().iAddr );
            TBTDeviceClass tmpDeviceClass( sa.MajorServiceClass(), sa.MajorClassOfDevice(), sa.MinorClassOfDevice());
            TUint16 msc(0);
            TUint8 cod(0);
            TUint8 codd(0);
            
            msc = tmpDeviceClass.MajorServiceClass();
            cod = tmpDeviceClass.MajorDeviceClass();
            codd = tmpDeviceClass.MinorDeviceClass();
            TUint32 whole(0);
            whole = tmpDeviceClass.DeviceClass();

            if(resp.IsValidBDAddr())
                {
                if(whole == KWiiMoteDeviceClass)
                    {
                    iObserver.DeviceSelected(resp.BDAddr(),TControllerDataTypes::ERemoteTypeWii,KErrNone);
                    }
                }
            else
                {
                iObserver.DeviceSelected(0,TControllerDataTypes::ERemoteTypeNoController,KErrNotFound);
                }
            }break;
            
        case EError:
            {
            
            }break;
            
        default:
            break;
        }
    }

void CDeviceFinder::ContinueDiscover()
    {
    iResolver.Next(iEntry,iStatus);
    iState = EDiscoverNoUi;
    SetActive();
    }

TInt CDeviceFinder::RunError(TInt aError)
    {
    if(aError == KErrHostResNoMoreResults)
        {
        iObserver.DeviceSelected(0,TControllerDataTypes::ERemoteTypeNoController,KErrHostResNoMoreResults);
        }
    return aError;
    }

void CDeviceFinder::StartDiscoveringL()
    {
    if(IsActive())
        Cancel();
    TProtocolDesc desc;
    iSockServ.Close();
    User::LeaveIfError(iSockServ.Connect());
    User::LeaveIfError(iSockServ.FindProtocol(KBTLinkManagerTxt(),desc));
    User::LeaveIfError(iResolver.Open(iSockServ,desc.iAddrFamily,desc.iProtocol));
        
    iAddr.SetIAC(KGIAC);
    iAddr.SetAction(KHostResInquiry|KHostResName|KHostResIgnoreCache);
    iResolver.GetByAddress(iAddr,iEntry,iStatus);
    iState = EDiscoverNoUi;
    SetActive();
    }

void CDeviceFinder::StartUiDiscoverL()
    {
    if(!iConnected)
        {
        User::LeaveIfError(iNotifier.Connect());
        iConnected = ETrue;
        }
       
    TBTDeviceResponseParams param;
    iResParams = param;
    iNotifier.StartNotifierAndGetResponse(iStatus,KDeviceSelectionNotifierUid,iSelParams,iResParams);
    iState = EDiscoverWithUi;
    SetActive();
    }
