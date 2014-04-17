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
#ifndef MBTCONTROLCALLBACKS_H_
#define MBTCONTROLCALLBACKS_H_

#include <bttypes.h>
#include <TControllerDataTypes.h>
#include "tremoteinfo.h"

class MBTControlCallbacks
    {
public:
    virtual void PowerState(TBool aState) = 0;
    
    virtual void DeviceSelected(TBTDevAddr aAddr, TControllerDataTypes::TRemoteType aType, TInt aError) = 0;
    
    virtual void ServiceSearchFinished(TInt aError) = 0;
    
    virtual void KeyConfigReady(TInt aError) = 0;
    
    virtual void SendUpdate(TRemoteInfo aInfo) = 0;
    
    virtual void ConnectDeviceComplete(TInt aError) = 0;
    
    virtual void DisconnectDeviceComplete(TInt aError = 0) = 0;
    };

#endif

