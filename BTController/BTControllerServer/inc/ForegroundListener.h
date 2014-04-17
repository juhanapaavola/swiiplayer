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
#ifndef FOREGROUNDLISTENER_H
#define FOREGROUNDLISTENER_H

#include <e32base.h>
#include <e32std.h>
#include <APGWGNAM.H> 
#include <APGTASK.H>
#include "MConnectionDataCallbacks.h"
#include "BLParser.h"
#include "MBlacklistParserObserver.h"


class CForegroundListener : public CActive, MBlacklistParserObserver
    {
public:

    ~CForegroundListener();

    static CForegroundListener* NewL(MConnectionDataCallbacks& aObserver);

    static CForegroundListener* NewLC(MConnectionDataCallbacks& aObserver);

    void GetUidConfig(TFileName& aFilename);
    
protected:
      
    void AddToBlacklist(TUint32 aItem);
    
    void ParserReady(TInt aError);
    
private:
    
    CForegroundListener(MConnectionDataCallbacks& aObserver);

    void ConstructL();

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);
    
    void Listen();

private:
    MConnectionDataCallbacks&   iObserver;
    RWsSession                  iWsSession;
    RWindowGroup*               iWindowGroup;
    CArrayFixFlat<TUid>*        iBlackListArray;
    CBLParser*					iBlacklistParser;
    TUid                        iCurrentUid;
    TInt						iParserReady;
    };

#endif

