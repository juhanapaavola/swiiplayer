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
#include "CommandCreator.h"

CCommandCreator::CCommandCreator(MConnectionDataCallbacks& aObserver):
iObserver(aObserver)
    {
    }

CCommandCreator::~CCommandCreator()
    {
    }

CCommandCreator* CCommandCreator::NewLC(MConnectionDataCallbacks& aObserver)
    {
    CCommandCreator* self = new (ELeave) CCommandCreator(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CCommandCreator* CCommandCreator::NewL(MConnectionDataCallbacks& aObserver)
    {
    CCommandCreator* self = CCommandCreator::NewLC(aObserver);
    CleanupStack::Pop();
    return self;
    }

void CCommandCreator::ConstructL()
    {
    }

void CCommandCreator::SendSettingWii(const TRemoteInfo& aInfo)
    {
    TBuf8<MAX_DATA_SIZE> tmpData;
    TInt len(0);
    
    TUint8* finalData = (TUint8*)User::AllocLC(MAX_DATA_SIZE);
    for(TInt i=0;i<MAX_DATA_SIZE;i++)
        {
        finalData[i] = 0;
        }
    
    TUint8 header(0);
    header = KWiiMoteSetReport|(TUint8)TControllerDataTypes::EWiiMoteOutput;
    finalData[0] = header;
    
    header = aInfo.iCommand;
    finalData[1] = header;
        
    switch(aInfo.iCommand)
        {
        case TControllerDataTypes::EWiiMoteCommandReportType:
            {
            TUint8 rep(0);
            TUint8 rep2(0);
            TBuf8<4> str;
            TPtr8 ptr(0,0);
            
            rep = aInfo.iWiiContinuousReport ? 0x04 : 0x00;
            rep2 = 0x00;
            
            if(aInfo.iWiiRumble)
                {
                rep |= 0x01;
                }
            if(aInfo.iWiiMotion)
                {
                rep2 = (TUint8)TControllerDataTypes::EWiiMoteReportButtonAcc;
                }
            else
                {
                rep2 = (TUint8)TControllerDataTypes::EWiiMoteReportButton;
                }
               
            finalData[2] = rep;
            finalData[3] = rep2;
            len=3;
            }break;
            
        case TControllerDataTypes::EWiiMoteCommandLED:            
            {
            TUint8 led(0);
            led = aInfo.iNumber & 0xF0;
            if(aInfo.iWiiRumble)
                {
                led |= 0x01;
                }
            led |= 0x02;
            finalData[2] = led;
            len=2;
            }break;
            
        case TControllerDataTypes::EWiiMoteCommandControlStatus:
            {
            TUint8 cmd(0);
            if(aInfo.iWiiRumble)
                {
                cmd |= 0x01;
                }
            finalData[2] = cmd;
            len=2;
            }break;
            
        case TControllerDataTypes::EWiiMoteCommandAudioEnable:
            {
            TUint8 cmd(0);
            if(aInfo.iWiiSpeakerEnabled == TControllerDataTypes::EWiiSpeakerEnabled)
                {
                cmd |= 0x04;
                }
            else
                {
                cmd &= 0x04;
                }
            
            if(aInfo.iWiiRumble)
                {
                cmd |= 0x01;
                }
            finalData[2] = cmd;
            len=2;
            }break;
            
        case TControllerDataTypes::EWiiMoteCommandAudioMute:
            {
            TUint8 cmd(0);
            cmd |= 0x04;
            
            if(aInfo.iWiiRumble)
                {
                cmd |= 0x01;
                }
            finalData[2] = cmd;
            len=2;
            }break;
            
        default:
            break;
        }

    len+=1;
    TUint8* final = (TUint8*)User::AllocLC(len);
    for(TInt i=0;i<len;i++)
        {
        final[i] = finalData[i];
        }    
    
    TPtr8 p(final,len,len);
    iObserver.CommandReady(p);
    CleanupStack::PopAndDestroy(final);
    CleanupStack::PopAndDestroy(finalData);
    }


void CCommandCreator::SendCommand(TControllerDataTypes::TCommandType aCommand)
    {
    HBufC8* data = HBufC8::NewLC(2);
    
    switch(aCommand)
        {
        case TControllerDataTypes::ECommandTypeRumbleOn:
            {
            
            }break;
        case TControllerDataTypes::ECommandTypeRumbleOff:
            {
            
            }break;
        default:
            break;
        }
    iObserver.CommandReady(data->Des());
    CleanupStack::PopAndDestroy(data);
    }
