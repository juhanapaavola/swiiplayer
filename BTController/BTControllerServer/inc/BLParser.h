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
#ifndef BLPARSER_H
#define BLPARSER_H

#include <e32base.h>
#include <e32std.h>
#include <xml\contenthandler.h>
#include <xml\parser.h>
#include <f32file.h>
#include "MBlacklistParserObserver.h"

using namespace Xml;

class CBLParser : public CActive, public MContentHandler
	{
public:
	~CBLParser();

	static CBLParser* NewL(MBlacklistParserObserver& aObserver);

	static CBLParser* NewLC(MBlacklistParserObserver& aObserver);

	void ParseFile(const TFileName& aFilename);
	
private:
	CBLParser(MBlacklistParserObserver& aObserver);

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
    MBlacklistParserObserver&	iObserver;
    CParser*             		iParser;
    HBufC8*              		iBuffer;
    RFs                     	iFs;
    RFile                		iFile;
    TFileName					iFilename;
	};

#endif // BLPARSER_H
