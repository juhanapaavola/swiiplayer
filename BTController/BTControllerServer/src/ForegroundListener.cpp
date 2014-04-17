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
#include "ForegroundListener.h"
#include "BTControllerCommon.h"


CForegroundListener::CForegroundListener(MConnectionDataCallbacks& aObserver) :
    CActive(EPriorityStandard),
    iObserver(aObserver),
    iWindowGroup(0),
    iParserReady(KErrNotReady)
    {
    }

CForegroundListener* CForegroundListener::NewLC(MConnectionDataCallbacks& aObserver)
    {
    CForegroundListener* self = new (ELeave) CForegroundListener(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CForegroundListener* CForegroundListener::NewL(MConnectionDataCallbacks& aObserver)
    {
    CForegroundListener* self = CForegroundListener::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CForegroundListener::ConstructL()
    {
    TFileName fname;
    CActiveScheduler::Add(this);
    
    iBlackListArray = new (ELeave)CArrayFixFlat<TUid>(3);    
    iBlacklistParser = CBLParser::NewL(*this);
    
    
    fname = KNullDesC;
    TInt err(KErrNone);
    RFs fs;
    fs.Connect();
    fs.PrivatePath(fname);
    TFindFile ff(fs);
    err = ff.FindByDir(fname,KNullDesC);
    if(err == KErrNone)
        {
        fname.Zero();
        fname = ff.File();
        }
    fname.Append(KAppBlackListFileName);        
    fs.Close();
    iBlacklistParser->ParseFile(fname);
        
    User::LeaveIfError( iWsSession.Connect() );
    iWindowGroup = new (ELeave) RWindowGroup ( iWsSession );
    
    User::LeaveIfError( iWindowGroup->Construct( (TUint32)iWindowGroup, EFalse ) );
    iWindowGroup->SetOrdinalPosition(-1);
    iWindowGroup->EnableReceiptOfFocus(EFalse);
    CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(iWsSession);
    wgName->SetHidden(ETrue);
    wgName->SetWindowGroupName(*iWindowGroup);
    CleanupStack::PopAndDestroy(1);
    User::LeaveIfError(iWindowGroup->EnableFocusChangeEvents());    
    }

CForegroundListener::~CForegroundListener()
    {
    Cancel();
    if(iBlacklistParser)
        {
        delete iBlacklistParser;
        iBlacklistParser = NULL;
        }
    
    if(iBlackListArray)
        {
        delete iBlackListArray;
        iBlackListArray = NULL;
        }
    
    if(iWindowGroup)
        {
        iWindowGroup->Close();
        delete iWindowGroup;
        iWindowGroup = NULL;
        }
    iWsSession.Close();
    }

void CForegroundListener::DoCancel()
    {
    iWsSession.EventReadyCancel();
    }

void CForegroundListener::RunL()
    {
    if(iStatus.Int() == KErrNone) 
        {
        TWsEvent event;
        iWsSession.GetEvent(event);
        TInt windowGroupId = iWsSession.GetFocusWindowGroup();
        CApaWindowGroupName* windowGroupName;
        windowGroupName = CApaWindowGroupName::NewLC(iWsSession, windowGroupId);
        iCurrentUid = windowGroupName->AppUid(); 
        CleanupStack::PopAndDestroy();
                
        for(TInt i=0;i<iBlackListArray->Count();i++)
            {
            if(iCurrentUid.iUid == iBlackListArray->At(i).iUid)
                {
                iObserver.SetForegroundSending(EFalse);
                break;
                }
            else
                {
                iObserver.SetForegroundSending(ETrue);
                }     
            }
        }
    Listen();
    }

TInt CForegroundListener::RunError(TInt aError)
    {
    Listen();
    return aError;
    }

void CForegroundListener::Listen()
    {
    if( IsActive() )
        Cancel();
    
    iWsSession.EventReady(&iStatus);
    SetActive();
    }   

void CForegroundListener::AddToBlacklist(TUint32 aItem)
    {
    TUid uid;
    uid.iUid = aItem;
    iBlackListArray->AppendL(uid);    
    }

void CForegroundListener::ParserReady(TInt aError)
	{
	if(aError == KErrNone)
		{
		iParserReady = aError;
		Listen();
		}
	}

void CForegroundListener::GetUidConfig(TFileName& aFilename)
    {
    TFileName tmp;    
    TFileName fname;
    fname = KNullDesC;
    TInt err(KErrNone);
    RFs fs;
    fs.Connect();
    
    fs.PrivatePath(fname);
    TFindFile ff(fs);
    err = ff.FindByDir(fname,KNullDesC);
    if(err == KErrNone)
        {
        fname.Zero();
        fname = ff.File();
        }
    fs.Close();
    tmp.Append(fname.Left(2));
    tmp.Append(_L("\\"));
    tmp.Append(KConfigDirectory);
    tmp.Append(_L("\\"));
    
    fname = tmp;
    
    tmp.Zero();
    tmp.Format(_L("%8x"),iCurrentUid.iUid);
    if(tmp.Length()<1)
        {
        fname = KNullDesC;
        }
    else
        {
        fname.Append(tmp);
        fname.Append(_L(".xml"));
        }
    aFilename = fname;
    }
