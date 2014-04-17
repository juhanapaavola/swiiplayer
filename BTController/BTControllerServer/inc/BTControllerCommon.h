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
#ifndef BTCONTROLLERCOMMON_H_
#define BTCONTROLLERCOMMON_H_

#include <bt_sock.h>
#include <tcontrollerdatatypes.h>
#include "buildflags.hrh"

#define MAX_DATA_SIZE 32

const TInt KFileBufferSize = 1024; // buffer size for file reading
_LIT8( KXmlMimeType, "text/xml" );
_LIT( KDefaultConfig, "defaultkeys.xml");
_LIT( KAppBlackListFileName,"blacklist.xml");
_LIT( KConfigDirectory,"data\\WiiControllerConfig");

// for xml parsing and keymapping
_LIT( KWiiButtonA,"WIIBTNA");
_LIT( KWiiButtonB,"WIIBTNB");
_LIT( KWiiButtonLeft,"WIIBTNLEFT");
_LIT( KWiiButtonRight,"WIIBTNRIGHT");
_LIT( KWiiButtonUp,"WIIBTNUP");
_LIT( KWiiButtonDown,"WIIBTNDOWN");
_LIT( KWiiButtonHome,"WIIBTNHOME");
_LIT( KWiiButton1,"WIIBTN1");
_LIT( KWiiButton2,"WIIBTN2");
_LIT( KWiiButtonMinus,"WIIBTNMINUS");
_LIT( KWiiButtonPlus,"WIIBTNPLUS");
_LIT( KBlacklist,"APPBLACKLIST");

const TUint KServerMajorVersionNumber = 0;
const TUint KServerMinorVersionNumber = 1;
const TUint KServerBuildVersionNumber = 1;

const TUid KHidPServiceUID = {0x0011};
const TUid KPowerModeSettingNotifierUid = {0x100059E2};

const TUint16 KWiiMoteMajorServiceClass = 0x25;
const TUint8 KWiiMoteMajorDeviceClass = 0x04;
const TUint8 KWiiMoteMinorDeviceClass = 0;
const TUint32 KWiiMoteDeviceClass = 0x2504;

const TUint KAccPropertyX = 0x0001;
const TUint KAccPropertyY = 0x0002;
const TUint KAccPropertyZ = 0x0003;

const TUint8 KWiiMoteContinuousReporting=0x04;
const TUint8 KWiiMoteSetReport=0x50;

_LIT(KServerName,"BTControllerServer");
_LIT(KServerNameExe,"BTControllerServer.exe");
_LIT(KBTLinkManagerTxt,"BTLinkManager");


enum TServerMessage {    
    EDiscoverController = 0,
    EFindAndConnect,
    EDisconnect,
    EStartServer,
    EStopServer,
    ESwitchPowerOn,
    ELoadKeyConfig,
    EControllerType,
    EBatteryStatus,
    EWiiSpeakerEnabled,
    ESetWiiSpeakerEnabled,
    ESetAccelerometerState,
    EGetCurrentConfig,
    ELoadConfigByUid,
    ECancelAll,
    ELastMessage            
};

#endif

