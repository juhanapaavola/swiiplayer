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
#ifndef TREMOTEINFO_H_
#define TREMOTEINFO_H_

#include <e32std.h>
#include <TControllerDataTypes.h>
#include <bt_sock.h>

class TRemoteInfo
    {
public:
    TRemoteInfo();
    
public:
    TBool           						iConnected;
    TControllerDataTypes::TRemoteType     	iType;
    TUint8          						iReportType;
    TUint8          						iWiiRumble;
    TUint8          						iWiiMotion;
    TControllerDataTypes::TWiiSpeaker     	iWiiSpeakerEnabled;
    TBool           						iWiiExtensionConnected;
    TUint8          						iNumber;
    TUint8          						iWiiContinuousReport;
    TL2CAPSockAddr  						iAddress;    
    TUint16         						iButtonsHeld;
    TUint16         						iButtonsReleased;
    TUint16         						iButtonsNow;
    TUint16         						iButtons;    
    TInt            						iBattery;
    TUint8          						iCommand;
    TBool           						iLoadConfigByUid;
    };

#endif

