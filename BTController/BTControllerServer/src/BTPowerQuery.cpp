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
#include "BTPowerQuery.h"

CBTPowerQuery::CBTPowerQuery(MBTControlCallbacks& aObserver):
    CActive(CActive::EPriorityStandard),
    iObserver(aObserver)
    {
    }

CBTPowerQuery::~CBTPowerQuery()
    {
    Cancel();
    if(iNotifyHandler)
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        iNotifyHandler = NULL;
        }
    if(iRepository)
        {
        delete iRepository;
        iRepository = NULL;
        }
    }

CBTPowerQuery* CBTPowerQuery::NewLC(MBTControlCallbacks& aObserver)
    {
    CBTPowerQuery* self = new (ELeave) CBTPowerQuery(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CBTPowerQuery* CBTPowerQuery::NewL(MBTControlCallbacks& aObserver)
    {
    CBTPowerQuery* self = CBTPowerQuery::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CBTPowerQuery::ConstructL()
    {
    CActiveScheduler::Add(this);
    iRepository = CRepository::NewL( KCRUidBluetoothPowerState );
    
    TInt state;
    iRepository->Get( KBTPowerState, state );
     
    if( state == EBTPowerOn )
        {
        iObserver.PowerState(ETrue);
        }
    else
        {
        iObserver.PowerState(EFalse);
        }
              
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this,*iRepository,CCenRepNotifyHandler::EIntKey,KBTPowerState);
    iNotifyHandler->StartListeningL();
    
    }

void CBTPowerQuery::TurnBtOn()
    {
    TInt err(0);
    err = iNotifier.Connect();
    User::LeaveIfError( err );
    TPckgBuf<TBool> dummy(ETrue);
    iNotifier.StartNotifierAndGetResponse(iStatus, KPowerModeSettingNotifierUid, dummy, iReply);
    iState = EAskingPower;
    iNotifyHandler->StopListening();
    SetActive();
    }

void CBTPowerQuery::PowerReply()
    {
    iState = EDummyReply;
    TRequestStatus* s = &iStatus;
    iStatus = KRequestPending;
    User::RequestComplete(s,KErrNone);
    SetActive();    
    }

void CBTPowerQuery::RunL()
    {
    switch(iState)
        {
        case EAskingPower:
            {
            iObserver.PowerState(iReply());
            iNotifier.Close();
            iNotifyHandler->StartListeningL();
            }break;
        case EDummyReply:
            {
            iObserver.PowerState(ETrue);
            }
        default:
            break;
        }
    }


void CBTPowerQuery::DoCancel()
    {
    iNotifier.CancelNotifier(KPowerModeSettingNotifierUid);
    iNotifier.Close();
    }

TInt CBTPowerQuery::RunError(TInt aError)
    {
    return aError;
    }


void  CBTPowerQuery::HandleNotifyInt (TUint32 aId, TInt aNewValue)
    {
    if(aId == KBTPowerState)
        {
        if(aNewValue == EBTPowerOn)
            {
            iObserver.PowerState(ETrue);
            }
        else
            {
            iObserver.PowerState(EFalse);
            }
        }
    }
 
 
void  CBTPowerQuery::HandleNotifyReal (TUint32 aId, TReal aNewValue)
    {
    (void)aId;
    (void)aNewValue;
    }
   
 
void  CBTPowerQuery::HandleNotifyString (TUint32 aId, const TDesC16 &aNewValue)
    {
    (void)aId;
    (void)aNewValue;
    }

 
void  CBTPowerQuery::HandleNotifyBinary (TUint32 aId, const TDesC8 &aNewValue)
    {
    (void)aId;
    (void)aNewValue;    
    }
   
 
void  CBTPowerQuery::HandleNotifyGeneric (TUint32 aId)
    {
    (void)aId;    
    }
 
 
void  CBTPowerQuery::HandleNotifyError (TUint32 aId, TInt aError, CCenRepNotifyHandler *aHandler)
    {
    (void)aId;
    (void)aError;
    (void)aHandler;
    }
