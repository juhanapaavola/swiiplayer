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
#ifndef BTSETTINGS_H
#define BTSETTINGS_H

#include <e32std.h>
#include <e32base.h>
#include <bttypes.h>
#include <btdevice.h>
#include <centralrepository.h>
#include <btserversdkcrkeys.h>
#include <cenrepnotifyhandler.h> 
#include "MBTControlCallbacks.h"
#include "btcontrollercommon.h"


class CBTPowerQuery : public CActive, MCenRepNotifyHandlerCallback 
    {
public:
    ~CBTPowerQuery();

    static CBTPowerQuery* NewL(MBTControlCallbacks& aObserver);

    static CBTPowerQuery* NewLC(MBTControlCallbacks& aObserver);

    void TurnBtOn();
    
    void PowerReply();

    void  HandleNotifyInt (TUint32 aId, TInt aNewValue);      
     
    void  HandleNotifyReal (TUint32 aId, TReal aNewValue);        
     
    void  HandleNotifyString (TUint32 aId, const TDesC16 &aNewValue);  
     
    void  HandleNotifyBinary (TUint32 aId, const TDesC8 &aNewValue);        
     
    void  HandleNotifyGeneric (TUint32 aId);      
     
    void  HandleNotifyError (TUint32 aId, TInt aError, CCenRepNotifyHandler *aHandler); 

private:

    CBTPowerQuery(MBTControlCallbacks& aObserver);

    void ConstructL();

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

private:
    enum TBTSettingsState
        {
        EAskingPower = 0,
        EDummyReply
        };
    
    MBTControlCallbacks&	iObserver;
    RNotifier 				iNotifier;
    TPckgBuf<TBool> 		iReply;
    TInt 					iState;
    CCenRepNotifyHandler*   iNotifyHandler;
    CRepository*            iRepository;
    };

#endif

