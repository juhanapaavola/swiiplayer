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
#ifndef MBTCONTROLLERSERVERSESSIONCALLBACKS_H_
#define MBTCONTROLLERSERVERSESSIONCALLBACKS_H_

#include <bttypes.h>
#include <bt_sock.h>
#include <TControllerDataTypes.h>

class MBTControllerServerSessionCallbacks
    {
public:
    
    virtual void Powered(TInt aPowerStatus) = 0;
    
    virtual void ShutdownComplete() = 0;
    
    virtual void ControllerDiscoverComplete(TInt aStatus, TBTDevAddr aAddr) = 0;
    
    virtual void ConnectToComplete(TInt aStatus) = 0;
    
    virtual void KeyConfigLoadComplete(TInt aError) = 0;
    
    virtual void GetBatteryStatusComplete(TInt aBatteryStatus, TInt aError) = 0;
    
    virtual void GetSpeakerEnabledComplete(TControllerDataTypes::TWiiSpeaker aEnabled, TInt aError) = 0;
        
    virtual void ErrorComplete(TInt aError) = 0;
    };

#endif

