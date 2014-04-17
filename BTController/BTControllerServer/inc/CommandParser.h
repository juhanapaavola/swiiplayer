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
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <e32base.h>
#include <e32std.h>
#include <bttypes.h>
#include <bt_sock.h>
#include <e32keys.h>
#include <e32hashtab.h>
#include "btcontrollercommon.h"
#include "mconnectiondatacallbacks.h"
#include "mxmlloadercallbacks.h"
#include "xmlloader.h"
#include "tremoteinfo.h"

class CCommandParser : public CActive, MXmlLoaderCallbacks
    {
public:
    enum TStates {
        ESelfLoadConfig,
        ESelfLoadConfigReady
    };

public:
    ~CCommandParser();

    static CCommandParser* NewL(MConnectionDataCallbacks& aObserver);

    static CCommandParser* NewLC(MConnectionDataCallbacks& aObserver);

    void DataReadyL(const TDesC8& aData);
    
    void LoadKeyConfig(const TDesC& aFilename, TStates aState = ESelfLoadConfigReady);
        
    void GetConfigName(TFileName& aFilename);
    
private:
    CCommandParser(MConnectionDataCallbacks& aObserver);

    void ConstructL();

    void HandleInterruptDataL();
    
    void HandleButtonReport(TUint16 aButton);
    
    void HandleAccReport(TUint16 aAccX, TUint16 aAccY, TUint16 aAccZ);
    
    TBool IsButtonPressed(TRemoteInfo& aInfo, TUint16 aButton);
    
    TBool IsButtonHeld(TRemoteInfo& aInfo, TUint16 aButton);
    
    TBool IsButtonJustPressed(TRemoteInfo& aInfo, TUint16 aButton);
    
    TBool IsButtonReleased(TRemoteInfo& aInfo, TUint16 aButton);
    
    TStdScanCode FindButton(TControllerDataTypes::TWiiMoteButtons aButton);
    
    void DefaultConfigName(TFileName& aFilename);
    
    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

    void ActivateSelf();
       
    
protected:
    
    void AddNewKeyConfig(TUint16 aWiiKey, TUint8 aSymbianKey);
    
    void ParserFinished(TInt aError);
    
    
private:
    
    TInt 								iState;
    MConnectionDataCallbacks&   		iObserver;
    TControllerDataTypes::TRemoteType	iType;
    TUint8*                     		iData;
    TPtr8                       		iDataPtr;
    RHashMap<TUint32,TStdScanCode>      iWiiKeyConfigMap;
    CXmlLoader*                 		iLoader;    
    };

#endif

