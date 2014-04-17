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
#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <e32base.h>
#include <e32std.h>
#include <f32file.h> 
#include <xml\contenthandler.h>
#include <xml\parser.h>
#include "mxmlloadercallbacks.h"

using namespace Xml;

class CXmlLoader : public CActive, MContentHandler
    {
public:
    virtual ~CXmlLoader();

    static CXmlLoader* NewL(MXmlLoaderCallbacks& aObserver, TFileName aFilename);

    static CXmlLoader* NewLC(MXmlLoaderCallbacks& aObserver, TFileName aFilename);

public:
    void StartParserL(const TDesC& aFilename);
    
    void GetCurrentConfigName(TFileName& aFilename);
        
private:
    CXmlLoader(MXmlLoaderCallbacks& aObserver, TFileName aFilename);

    void ConstructL();

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

private: // from MContentHandler
 
    void OnStartDocumentL( const RDocumentParameters &aDocParam, TInt aErrorCode );
 
    void OnEndDocumentL( TInt aErrorCode );
 
    void OnStartElementL( const RTagInfo &aElement, const RAttributeArray &aAttributes, TInt aErrorCode );
 
    void OnEndElementL( const RTagInfo &aElement, TInt aErrorCode );
 
    void OnContentL( const TDesC8 &aBytes, TInt aErrorCode );
 
    void OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri, TInt aErrorCode );
 
    void OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode );
 
    void OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode );
 
    void OnSkippedEntityL( const RString &aName, TInt aErrorCode );
 
    void OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode);
 
    void OnError( TInt aErrorCode );
 
    TAny *GetExtendedInterface( const TInt32 aUid );
 
    
private:
    enum TXmlLoaderState
        {
        EUninitialized,
        EInitialized,
        EError
        };

private:
    TInt iState;
    MXmlLoaderCallbacks&	iObserver;
    CParser*             	iParser;
    HBufC8*              	iBuffer;
    RFs                     iFs;
    RFile                	iFile;
    TFileName       		iFilename;
    TFileName       		iDefaultConfig;
    TInt					iLoadError;
    };

#endif
