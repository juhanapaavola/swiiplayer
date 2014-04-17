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
#ifndef TCONTROLLERDATATYPES_H_
#define TCONTROLLERDATATYPES_H_

class TControllerDataTypes
{
public:
	
    enum TWiiSpeaker
         {
        EWiiSpeakerEnabled = 0,
        EWiiSpeakerDisabled,
        EWiiSpeakerMute,
        EWiiSpeakerError
        };

    enum TChannelType
        {
            EChannelTypeInterrupt = 0,
            EChannelTypeControl
        
        };

    enum TRemoteType         
        {
            ERemoteTypeWii = 0,
                    ERemoteTypeNoController
        };
    

    enum TCommandType
        {
        ECommandTypeRumbleOn = 0,
                ECommandTypeRumbleOff
    };

    enum TCommandSettingType
        {    
            ECommandSettingTypeInitRumbleOn = 0,
                    ECommandSettingTypeInitRumbleOff,
                    ECommandSettingTypeInitACCEnable,
                    ECommandSettingTypeInitACCDisable
    };

    enum TButtonClick
        {
            EButtonClickRepeat,
            EButtonClickReleased,
            EButtonClickPressed,
            EButtonClickJustPressed       
        };

    enum TWiiMoteLeds
        {
        EWiiMoteLedNone=0x00,
        EWiiMoteLed1=0x10,
        EWiiMoteLed2=0x20,
        EWiiMoteLed3=0x40,
        EWiiMoteLed4=0x80
        };
    
    enum TWiiMoteCommands
        {
        EWiiMoteCommandNone = 0,
        EWiiMoteCommandLED=0x11,
        EWiiMoteCommandReportType,
        EWiiMoteCommandRumble,
        EWiiMoteCommandIR = 0x13,             
        EWiiMoteCommandAudioEnable,
        EWiiMoteCommandControlStatus,
        EWiiMoteCommandWriteData,
        EWiiMoteCommandReadData,
        EWiiMoteCommandAudioMute,
        EWiiMoteCommandIR_2=0x1A        
        };
    
    enum TWiiMoteReports
        {
        EWiiMoteReportControlStatus=0x20,
        EWiiMoteReportRead,
        EWiiMoteReportWrite,
        EWiiMoteReportButton=0x30,
        EWiiMoteReportButtonAcc,
        EWiiMoteReportButtonAccIr=0x33,
        EWiiMoteReportButtonExp,
        EWiiMoteReportButtonAccExp,
        EWiiMoteReportButtonIrExp,
        EWiiMoteReportButtonAccIrExp        
        };
    
    enum TWiiMoteDataDirection
        {
        EWiiMoteInput=0x01,
        EWiiMoteOutput
        };
    
    enum TWiiMoteControlStatus
        {
        EWiiMoteControlStatusAttachment=0x02,
        EWiiMoteControlStatusSpeakerEnabled=0x04,
        EWiiMoteControlStatusIrEnabled=0x08,
        EWiiMoteControlStatusLED1=0x10,
        EWiiMoteControlStatusLED2=0x20,
        EWiiMoteControlStatusLED3=0x40,
        EWiiMoteControlStatusLED4=0x80,        
        EWiiMoteControlStatusBattery=0xC8
        };
    
    enum TWiiMoteButtons
        {
        EWiiMoteButtonReleased=0,
        EWiiMoteButtonTwo,
        EWiiMoteButtonOne,
        EWiiMoteButtonB=0x0004,
        EWiiMoteButtonA=0x0008,
        EWiiMoteButtonMinus=0x0010,
        EWiiMoteButtonZaccelBIT6=0x0020,
        EWiiMoteButtonZaccelBIT7=0x0040,
        EWiiMoteButtonHome=0x0080,
        EWiiMoteButtonLeft=0x0100,
        EWiiMoteButtonRight=0x0200,
        EWiiMoteButtonDown=0x0400,
        EWiiMoteButtonUp=0x0800,
        EWiiMoteButtonPlus=0x1000,
        EWiiMoteButtonZaccelBIT4=0x2000,
        EWiiMoteButtonZaccelBIT5=0x4000,
        EWiiMoteButtonUnknown=0x8000,
        EWiiMoteButtonAll=0x1F9F,        
        };
};

#endif

