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
#include "CommandParser.h"
#include <eikapp.h>
#include <e32math.h>
#include "connectiondata.h"

TUint32 HashFunc(const TUint32& aValue)
    {    
    return DefaultHash::Integer( (TInt)aValue);    
    }

TBool IdentityFunc(const TUint32& aAddr1, const TUint32& aAddr2)
    {    
    return DefaultIdentity::Integer( (TInt)aAddr1,(TInt)aAddr2 );
    }

CCommandParser::CCommandParser(MConnectionDataCallbacks& aObserver) :
    CActive(EPriorityStandard),
    iObserver(aObserver),
    iType(TControllerDataTypes::ERemoteTypeNoController),
    iDataPtr(0,0),
    iWiiKeyConfigMap(&HashFunc,&IdentityFunc)
    {
    }

CCommandParser* CCommandParser::NewLC(MConnectionDataCallbacks& aObserver)
    {
    CCommandParser* self = new (ELeave) CCommandParser(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CCommandParser* CCommandParser::NewL(MConnectionDataCallbacks& aObserver)
    {
    CCommandParser* self = CCommandParser::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CCommandParser::ConstructL()
    {
    CActiveScheduler::Add(this);

    TFileName fname;
    DefaultConfigName(fname);
    
    iLoader = CXmlLoader::NewL(*this,fname);
    LoadKeyConfig(KNullDesC,ESelfLoadConfig);           
    }

void CCommandParser::DefaultConfigName(TFileName& aFilename)
	{
    TBuf<KMaxFileName> fname;
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
    fname.Append(KDefaultConfig);    
    fs.Close();

    aFilename = fname;
	}

    

CCommandParser::~CCommandParser()
    {
    Cancel();
    if(iData)
        {
        User::Free(iData);
        }
    iWiiKeyConfigMap.Close();
    
    if(iLoader)
        {
        delete iLoader;
        iLoader = NULL;
        }
    }

void CCommandParser::DoCancel()
    {
    if(iData)
        {
        User::Free(iData);
        }
    }


void CCommandParser::RunL()
    {
    RDebug::Print(_L("CCommandParser::RunL report: %X"),iState);
    switch(iState)
        {
        case TControllerDataTypes::EWiiMoteReportButton:
            {
            TUint16 btn(0);
            btn = iDataPtr[2];
            btn = iDataPtr[2] << 8;
            btn |= iDataPtr[3];
            HandleButtonReport(btn);            
            }break;

        case TControllerDataTypes::EWiiMoteReportButtonAcc:
            {
            TUint16 btn(0);
            TUint16 accX(0);
            TUint16 accY(0);
            TUint16 accZ(0);
            btn = iDataPtr[2];
            btn = btn << 8;
            btn |= iDataPtr[3];
            HandleButtonReport(btn);
            
            accX = iDataPtr[4];
            accY = iDataPtr[5];
            accZ = iDataPtr[6];
            
            TUint8 lsb1(0);
            TUint8 lsb2(0);
            TUint8 lsb3(0);
            TUint8 lsb4(0);
            
            lsb1 = iDataPtr[2] & 0x40;
            lsb2 = iDataPtr[2] & 0x20;
            lsb3 = iDataPtr[3] & 0x40;
            lsb4 = iDataPtr[3] & 0x20;
            RDebug::Print(_L("XLRLSB: %X , %d - XLRLSB: %X , %d"),lsb1,lsb1,lsb2,lsb2);
            RDebug::Print(_L("XLRLSB: %X , %d - XLRLSB: %X , %d"),lsb3,lsb3,lsb4,lsb4);
            RDebug::Print(_L("AccX: %X , %d - AccY: %X , %d - AccZ: %X , %d"),accX,accX,accY,accY,accZ,accZ);
            
            HandleAccReport(accX,accY,accZ);
            }break;
            
        case TControllerDataTypes::EWiiMoteReportControlStatus:
            {
            TUint8 data(0);
            data = iDataPtr[7];
            
            TRemoteInfo info;
            info = iObserver.RemoteInfo();
            info.iCommand = (TControllerDataTypes::TWiiMoteCommands)iState;
            TReal32 d(data);
            TReal32 b(TControllerDataTypes::EWiiMoteControlStatusBattery);
            TInt16 bat=0;
            TReal calc( (d/b)*100);
            Math::Int(bat,calc);
            info.iBattery = bat;
            iObserver.UpdateRemoteInfo(info);
            }break;
            
        case TControllerDataTypes::EWiiMoteReportWrite:
            {
            TUint8 data1(0);
            TUint8 data2(0);
            TUint8 data3(0);
            TUint8 data4(0);
            data1 = iDataPtr[0];
            data2 = iDataPtr[1];
            data3 = iDataPtr[2];
            data4 = iDataPtr[3];

            TRemoteInfo info;
            info = iObserver.RemoteInfo();
            info.iCommand = (TControllerDataTypes::TWiiMoteCommands)data3;
            switch(data3)
                {
                case TControllerDataTypes::EWiiMoteCommandAudioEnable:
                case TControllerDataTypes::EWiiMoteCommandAudioMute:
                    {
                    if(data1 || data2 || data4)
                        {
                        info.iWiiSpeakerEnabled = TControllerDataTypes::EWiiSpeakerError;
                        }
                    }break;
                }            
            iObserver.UpdateRemoteInfo(info);
            }break;
        default:
            break;
        }
    }

TInt CCommandParser::RunError(TInt aError)
    {
    return aError;
    }

void CCommandParser::DataReadyL(const TDesC8& aData)
    {
    if(IsActive())
        {
        return;
        }
    
    if(iData)
        {
        User::Free(iData);
        iData = NULL;
        }
    iData = (TUint8*)User::AllocL(aData.Length());
    iDataPtr.Set(iData,aData.Length(),aData.Length());
    for(TInt i=0;i<aData.Length();i++)
        {
        iDataPtr[i] = aData[i];
        }
    iState = iDataPtr[1];
    ActivateSelf();
    }

void CCommandParser::ActivateSelf()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    SetActive();
    }


void CCommandParser::HandleInterruptDataL()
    {
    }

void CCommandParser::HandleButtonReport(TUint16 aButton)
    {
    TUint16 all(0);
    TRemoteInfo info;    
    info = iObserver.RemoteInfo();
    
    all = aButton & TControllerDataTypes::EWiiMoteButtonAll;
    info.iButtonsHeld = (all & info.iButtons);
    info.iButtonsReleased = ((info.iButtons | info.iButtonsHeld) & ~all);
    info.iButtons = all;            

    iObserver.UpdateRemoteInfo(info);
    
    // button held will be button with repeat flag
    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonOne))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonOne),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonTwo))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonTwo),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonDown))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonDown),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonUp))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonUp),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonLeft))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonLeft),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonRight))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonRight),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonB))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonB),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonA))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonA),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonPlus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonPlus),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonMinus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonMinus),TControllerDataTypes::EButtonClickRepeat);

    if(IsButtonHeld(info,TControllerDataTypes::EWiiMoteButtonHome))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonHome),TControllerDataTypes::EButtonClickRepeat);
    
    // just pressed
    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonOne))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonOne),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonTwo))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonTwo),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonDown))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonDown),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonUp))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonUp),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonLeft))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonLeft),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonRight))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonRight),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonB))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonB),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonA))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonA),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonPlus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonPlus),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonMinus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonMinus),TControllerDataTypes::EButtonClickJustPressed);

    if(IsButtonJustPressed(info,TControllerDataTypes::EWiiMoteButtonHome))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonHome),TControllerDataTypes::EButtonClickJustPressed);
    
    // check released buttons    
    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonOne))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonOne),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonTwo))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonTwo),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonDown))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonDown),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonUp))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonUp),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonLeft))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonLeft),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonRight))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonRight),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonB))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonB),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonA))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonA),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonPlus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonPlus),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonMinus))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonMinus),TControllerDataTypes::EButtonClickReleased);

    if(IsButtonReleased(info,TControllerDataTypes::EWiiMoteButtonHome))
    	iObserver.SendButtonClick( FindButton(TControllerDataTypes::EWiiMoteButtonHome),TControllerDataTypes::EButtonClickReleased);
    }

void CCommandParser::HandleAccReport(TUint16 aAccX, TUint16 aAccY, TUint16 aAccZ)
    {
    iObserver.SendAccValues( aAccX,  aAccY,  aAccZ);
    }

TStdScanCode CCommandParser::FindButton(TControllerDataTypes::TWiiMoteButtons aButton)
    {
    TStdScanCode code(EStdKeyNull);
    TInt err(KErrNotFound);
    TRAP(err, code = iWiiKeyConfigMap.FindL((TUint16)aButton));            
    if(err != KErrNone)
        {
        code = EStdKeyNull;
        }    
    return code;
    }

TBool CCommandParser::IsButtonPressed(TRemoteInfo& aInfo, TUint16 aButton)
    {
    TBool ret(EFalse);
    if((aInfo.iButtons & aButton) == aButton)
        ret = ETrue;
    return ret;
    }

TBool CCommandParser::IsButtonHeld(TRemoteInfo& aInfo, TUint16 aButton)
    {
    TBool ret(EFalse);
    if((aInfo.iButtonsHeld & aButton) == aButton)
        ret = ETrue;
    return ret;        
    }

TBool CCommandParser::IsButtonReleased(TRemoteInfo& aInfo, TUint16 aButton)
    {
    TBool ret(EFalse);
    if((aInfo.iButtonsReleased & aButton) == aButton)
        ret = ETrue;
    return ret;    
    }

TBool CCommandParser::IsButtonJustPressed(TRemoteInfo& aInfo, TUint16 aButton)
    {
    TBool ret(EFalse);
    if( IsButtonPressed(aInfo,aButton) && !IsButtonHeld(aInfo,aButton))
        ret = ETrue;
    return ret;    
    }

void CCommandParser::LoadKeyConfig(const TDesC& aFilename, TStates aState)
    {
    if(aState)
        {
        iState = aState;
        }
    if(iLoader)
        {
        iLoader->Cancel();
        }
    else
        {
		TFileName fname;
        DefaultConfigName(fname);
        iLoader = CXmlLoader::NewL(*this,fname);
        }
    iLoader->StartParserL(aFilename);
    }

void CCommandParser::AddNewKeyConfig(TUint16 aWiiKey, TUint8 aSymbianKey)
    {
    if(iWiiKeyConfigMap.Find(aWiiKey) != NULL)
        {        
        iWiiKeyConfigMap.Remove(aWiiKey);
        }
    iWiiKeyConfigMap.Insert(aWiiKey,(TStdScanCode)aSymbianKey);
    }

void CCommandParser::ParserFinished(TInt aError)
    {
    switch(iState)
        {
        case ESelfLoadConfigReady:
            {
            iObserver.KeyConfigReady(aError);
            }break;
        case ESelfLoadConfig:        
            {
            iState = ESelfLoadConfigReady;
            }break;
        default:
            break;
        }
    }


void CCommandParser::GetConfigName(TFileName& aFilename)
    {
    iLoader->GetCurrentConfigName(aFilename);
    }
	
