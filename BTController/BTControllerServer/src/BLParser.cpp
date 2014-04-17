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
#include "BLParser.h"
#include "BTControllerCommon.h"

CBLParser::CBLParser(MBlacklistParserObserver& aObserver) :
	CActive(CActive::EPriorityHigh),
	iObserver(aObserver)
	{
	}

CBLParser* CBLParser::NewLC(MBlacklistParserObserver& aObserver)
	{
	CBLParser* self = new (ELeave) CBLParser(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CBLParser* CBLParser::NewL(MBlacklistParserObserver& aObserver)
	{
	CBLParser* self = CBLParser::NewLC(aObserver);
	CleanupStack::Pop();
	return self;
	}

void CBLParser::ConstructL()
	{
    CActiveScheduler::Add(this);
    iParser = CParser::NewL(KXmlMimeType,*this);
    iFs.Connect();
	}

CBLParser::~CBLParser()
	{
	Cancel();
    if(iParser)
        {
        delete iParser;
        iParser = NULL;
        }
    if(iBuffer)
        {
        delete iBuffer;
        iBuffer = NULL;
        }
    iFile.Close();
    iFs.Close();			
	}

void CBLParser::DoCancel()
	{
	iParser->ParseEndL();
    iFile.Close();
    if(iBuffer)
        {
        delete iBuffer;
        iBuffer = NULL;
        }	
	}

void CBLParser::RunL()
	{	
    if(iStatus.Int() == KErrNone)
        {
        if(iBuffer->Length() == 0)
            {
            iParser->ParseEndL();
            
            if(iBuffer)
                {
                delete iBuffer;
                iBuffer = NULL;
                }
            iFile.Close();
            iObserver.ParserReady(KErrNone);
            }
        else
            {
            iParser->ParseL(*iBuffer);
            
            TPtr8 ptr(iBuffer->Des());
            TInt size(0);
            iFile.Size(size);
            iFile.Read(ptr,size,iStatus);
            SetActive();
            }
        }
    else
        {
    	iObserver.ParserReady(KErrGeneral);
        }	
	}

TInt CBLParser::RunError(TInt aError)
	{
	return aError;
	}


void CBLParser::ParseFile(const TFileName& aFilename)
	{
	iFilename = aFilename;
	TInt err(KErrNone);
    if(IsActive())
        {
        Cancel();
        }
    
    err = iFile.Open(iFs,iFilename,EFileRead);
    if(err != KErrNone)
        {
        iFile.Close();
        return;
        }

    if(iBuffer)
        {
        delete iBuffer;
        iBuffer = NULL;
        }
    TInt size(0);
    iFile.Size(size);
    iBuffer = HBufC8::NewL(size);
    TPtr8 ptr(iBuffer->Des());
    iFile.Read(ptr,size,iStatus);
    
    SetActive();
    iParser->ParseBeginL();    
	}


void CBLParser::OnStartDocumentL( const RDocumentParameters &aDocParam, TInt aErrorCode )
    {
    (void)aDocParam;
    (void)aErrorCode;
    }

void CBLParser::OnEndDocumentL( TInt aErrorCode )
    {
	(void)aErrorCode;
    }

void CBLParser::OnStartElementL( const RTagInfo &aElement, const RAttributeArray &aAttributes, TInt aErrorCode )
    {
    (void)aErrorCode;
    TBuf<KMaxFileName> name;
    RAttribute attr;
    TBuf<10> num;
    TLex lex;
    TInt err(KErrNone);
    TUint32 scan(0);    
    
    name.Copy(aElement.LocalName().DesC());
    if(aAttributes.Count())
        {
        attr = aAttributes[0];
        num.Copy(attr.Value().DesC());
        lex.Assign(num);
        
        if(name == KBlacklist)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddToBlacklist(scan);
            }
        }    
    }

void CBLParser::OnEndElementL( const RTagInfo &aElement, TInt aErrorCode )
    {
    (void)aElement;
    (void)aErrorCode;
    }

void CBLParser::OnContentL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    (void)aBytes;
    (void)aErrorCode;
    }

void CBLParser::OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri, TInt aErrorCode )
    {
    (void)aPrefix;
    (void)aUri;
    (void)aErrorCode;
    }

void CBLParser::OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode )
    {
    (void)aPrefix;
    (void)aErrorCode;
    }

void CBLParser::OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    (void)aBytes;
    (void)aErrorCode;    
    }

void CBLParser::OnSkippedEntityL( const RString &aName, TInt aErrorCode )
    {
    (void)aName;
    (void)aErrorCode;
    }

void CBLParser::OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode)
    {
    (void)aTarget;
    (void)aData;
    (void)aErrorCode;
    }

void CBLParser::OnError( TInt aErrorCode )
    {
    (void)aErrorCode;
    }

TAny* CBLParser::GetExtendedInterface( const TInt32 aUid )
    {
    (void)aUid;
    return NULL;
    }

