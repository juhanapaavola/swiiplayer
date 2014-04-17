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
#include "XmlLoader.h"
#include <e32const.h>
#include <stringpool.h>
#include "btcontrollercommon.h"

CXmlLoader::CXmlLoader(MXmlLoaderCallbacks& aObserver, TFileName aFilename) :
    CActive(EPriorityStandard),
    iObserver(aObserver),
    iDefaultConfig(aFilename)
    {
    }

CXmlLoader* CXmlLoader::NewLC(MXmlLoaderCallbacks& aObserver, TFileName aFilename)
    {
    CXmlLoader* self = new (ELeave) CXmlLoader(aObserver,aFilename);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CXmlLoader* CXmlLoader::NewL(MXmlLoaderCallbacks& aObserver, TFileName aFilename)
    {
    CXmlLoader* self = CXmlLoader::NewLC(aObserver,aFilename);
    CleanupStack::Pop();
    return self;
    }

void CXmlLoader::ConstructL()
    {
    CActiveScheduler::Add(this);
    iParser = CParser::NewL(KXmlMimeType,*this);
    iFs.Connect();
    }

CXmlLoader::~CXmlLoader()
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

void CXmlLoader::DoCancel()
    {
    iParser->ParseEndL();
    iFile.Close();
    if(iBuffer)
        {
        delete iBuffer;
        iBuffer = NULL;
        }
    }

void CXmlLoader::StartParserL(const TDesC& aFilename)
    {
    iFilename = aFilename;
    iLoadError = KErrNone;
    if(IsActive())
        {
        Cancel();
        }
    
    iLoadError = iFile.Open(iFs,iFilename,EFileRead);
    if(iLoadError != KErrNone)
        {
        // load default
        iFile.Close();
        iFilename = iDefaultConfig;
        TInt err(KErrNone);
        err = iFile.Open(iFs,iFilename,EFileRead);
        if(err != KErrNone)
        	{        	
        	iObserver.ParserFinished(err);
        	return;
        	}
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

void CXmlLoader::RunL()
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
            }
        else
            {
            iParser->ParseL(*iBuffer);
            
            TPtr8 ptr(iBuffer->Des());
            ptr.Zero();
            TInt size(0);
            iFile.Size(size);
            iFile.Read(ptr,size,iStatus);
            SetActive();
            }
        }
    else
        {
        // TODO: Error handling
        }
    }

TInt CXmlLoader::RunError(TInt aError)
    {
    return aError;
    }

void CXmlLoader::OnStartDocumentL( const RDocumentParameters &aDocParam, TInt aErrorCode )
    {
    (void)aDocParam;
    (void)aErrorCode;
    }

void CXmlLoader::OnEndDocumentL( TInt aErrorCode )
    {
    TInt err(KErrNone);
    if(iLoadError != KErrNone)
    	err = iLoadError;
    else
    	err = aErrorCode;
    
    iObserver.ParserFinished(err);
    }

void CXmlLoader::OnStartElementL( const RTagInfo &aElement, const RAttributeArray &aAttributes, TInt aErrorCode )
    {
    (void)aErrorCode;
    TBuf<KMaxFileName> name;
    RAttribute attr;
    TBuf<10> num;
    TLex lex;
    TInt err(KErrNone);
    TUint8 scan(0);    
    
    name.Copy(aElement.LocalName().DesC());
    if(aAttributes.Count())
        {
        attr = aAttributes[0];
        num.Copy(attr.Value().DesC());
        TInt locate(KErrNotFound);
        _LIT(KHexPrefix,"0x");
        locate = num.Find(KHexPrefix);
        if(locate != KErrNotFound)
        	{
        	num.Delete(locate,KHexPrefix().Length());
        	}
        
        lex.Assign(num);
                
        if(name == KWiiButtonA)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonA,scan);
            }

        if(name == KWiiButtonB)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonB,scan);
            }

        if(name == KWiiButtonLeft)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonLeft,scan);
            }

        if(name == KWiiButtonRight)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonRight,scan);
            }

        if(name == KWiiButtonUp)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonUp,scan);
            }

        if(name == KWiiButtonDown)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonDown,scan);
            }

        if(name == KWiiButtonHome)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonHome,scan);
            }

        if(name == KWiiButton1)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonOne,scan);
            }

        if(name == KWiiButton2)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonTwo,scan);
            }
        
        if(name == KWiiButtonPlus)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonPlus,scan);
            }

        if(name == KWiiButtonMinus)
            {
            err = lex.Val(scan,EHex);
            iObserver.AddNewKeyConfig(TControllerDataTypes::EWiiMoteButtonMinus,scan);
            }
        }    
    }

void CXmlLoader::OnEndElementL( const RTagInfo &aElement, TInt aErrorCode )
    {
    (void)aElement;
    (void)aErrorCode;
    }

void CXmlLoader::OnContentL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    (void)aBytes;
    (void)aErrorCode;
    }

void CXmlLoader::OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri, TInt aErrorCode )
    {
    (void)aPrefix;
    (void)aUri;
    (void)aErrorCode;
    }

void CXmlLoader::OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode )
    {
    (void)aPrefix;
    (void)aErrorCode;
    }

void CXmlLoader::OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    (void)aBytes;
    (void)aErrorCode;    
    }

void CXmlLoader::OnSkippedEntityL( const RString &aName, TInt aErrorCode )
    {
    (void)aName;
    (void)aErrorCode;
    }

void CXmlLoader::OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode)
    {
    (void)aTarget;
    (void)aData;
    (void)aErrorCode;
    }

void CXmlLoader::OnError( TInt aErrorCode )
    {
    (void)aErrorCode;
    }

TAny* CXmlLoader::GetExtendedInterface( const TInt32 aUid )
    {
    (void)aUid;
    return NULL;
    }

void CXmlLoader::GetCurrentConfigName(TFileName& aFilename)
    {
    aFilename = iFilename;
    }
