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
#ifndef COMMANDCREATOR_H
#define COMMANDCREATOR_H

#include <e32std.h>
#include <e32base.h>
#include "btcontrollercommon.h"
#include "mconnectiondatacallbacks.h"
#include "tremoteinfo.h"

class CCommandCreator : public CBase
    {
public:
    ~CCommandCreator();

    static CCommandCreator* NewL(MConnectionDataCallbacks& aObserver);

    static CCommandCreator* NewLC(MConnectionDataCallbacks& aObserver);

    void SendSettingWii(const TRemoteInfo& aInfo);
    
    void SendCommand(TControllerDataTypes::TCommandType aCommand);
    
private:

    CCommandCreator(MConnectionDataCallbacks& aObserver);

    void ConstructL();

private:
    MConnectionDataCallbacks&   iObserver;
    HBufC8*                     iData;
    };

#endif

