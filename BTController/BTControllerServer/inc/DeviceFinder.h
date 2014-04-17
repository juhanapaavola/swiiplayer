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
#ifndef DEVICEFINDER_H
#define DEVICEFINDER_H

#include <e32base.h>
#include <e32std.h>
#include <es_sock.h>
#include <bt_sock.h>
#include <btextnotifiers.h>
#include <btdevice.h>
#include "MBTControlCallbacks.h"
#include "btcontrollercommon.h"

class CDeviceFinder : public CActive
    {
public:
    ~CDeviceFinder();

    static CDeviceFinder* NewL(MBTControlCallbacks& aObserver);

    static CDeviceFinder* NewLC(MBTControlCallbacks& aObserver);

public:
    void StartDiscoveringL();
    
    void StartUiDiscoverL();
    
    void ContinueDiscover();
    
private:
    CDeviceFinder(MBTControlCallbacks& aObserver);

    void ConstructL();

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

private:
    enum TDeviceFinderState
        {
        EUninitialized = 0,
        EInitialized,
        EDiscoverNoUi,
        EDiscoverWithUi,
        EError
        };

private:
    TInt 							iState;    
    MBTControlCallbacks&            iObserver;
    RSocketServ                     iSockServ;
    RHostResolver                   iResolver;
    RNotifier                       iNotifier;
    TBTDeviceSelectionParamsPckg    iSelParams;
    TBTDeviceResponseParamsPckg     iResParams;
    TBool                           iConnected;
    TInquirySockAddr                iAddr;
    TNameEntry                      iEntry;
    
    };

#endif
