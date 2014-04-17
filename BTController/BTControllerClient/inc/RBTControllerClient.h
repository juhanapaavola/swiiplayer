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
#ifndef RBTCONTROLLERCLIENT_H
#define RBTCONTROLLERCLIENT_H

#include <e32std.h>
#include <e32base.h>
#include <bttypes.h>
#include <TControllerDataTypes.h>
#include <bt_sock.h>


class RBTControllerClient : public RSessionBase
    {
public:
    IMPORT_C ~RBTControllerClient();

    IMPORT_C RBTControllerClient();

public:
    
    IMPORT_C TInt Connect();
    
    IMPORT_C void Close();
    
    IMPORT_C TVersion Version() const;
    
    IMPORT_C void Start();
    
    /*
     *  Asynchronous calls
     */
    
    /*
     * Request power on to phone Bluetooth
     * Return: Symbian standard error code in aStatus
     */
    IMPORT_C void Power(TRequestStatus& aStatus);
    
    /*
     * FindAndConnect
     * Discover BT device and connects to it
     * Return: aStatus contains standard Symbian error code
     */
    IMPORT_C void FindAndConnect(TRequestStatus& aStatus);
    
    /*
     * Disconnect
     * Disconnects controller from server
     */
    IMPORT_C void Disconnect();
               
    /*
     * LoadKeyConfig, loads XML what contains key mapping codes
     * Return: Symbian standard error code in aStatus
     */
    IMPORT_C void LoadKeyConfig(const TDes& aFilename, TRequestStatus& aStatus);
    
    /*
     * ControllerType
     * Return: aType contains type of the remote device, Symbian standard error code in aStatus
     */
    IMPORT_C void ControllerType(TControllerDataTypes::TRemoteType& aType, TRequestStatus& aStatus);
    
    /*
     * BatteryStatus
     * Return: aBatteryStatus as integer of remote device battery level, Symbian standard error code in aStatus
     */
    IMPORT_C void BatteryStatus(TInt& aBatteryStatus, TRequestStatus& aStatus);
    
    /*
     * WiiSpeakerStatus
     * Return: aSpeakerStatus shows status of remote device speaker, Symbian standard error code in aStatus
     */
    IMPORT_C void WiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker& aSpeakerStatus, TRequestStatus& aStatus);
    
    /*
     * SetWiiSpeakerStatus
     * Param: aSpeakerStatus is state to be set
     * Return: Symbian standard error code in aStatus
     */
    IMPORT_C void SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aSpeakerStatus, TRequestStatus& aStatus);
    
    /*
     * SetWiiAccelerometerState
     * Param: aState is true/false, either send Accelerometer to PS key or do not send anything
     * Return: Symbian standard error code in aStatus
     */
    IMPORT_C void SetWiiAccelerometerState(TBool aState, TRequestStatus& aStatus);
    
    /*
     * Synchronous calls
     */
    IMPORT_C void Power();
                
    IMPORT_C void LoadKeyConfig(const TFileName& aFilename);
    
    IMPORT_C void ControllerType(TControllerDataTypes::TRemoteType& aType);
    
    IMPORT_C void BatteryStatus(TInt& aBatteryStatus);
    
    IMPORT_C void WiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker& aSpeakerStatus);
    
    IMPORT_C void SetWiiSpeakerStatus(TControllerDataTypes::TWiiSpeaker aSpeakerStatus);
    
    IMPORT_C void SetWiiAccelerometerState(TBool aState);
    
    /*
     * SetLoadConfigByUid
     * Param: change keyconfig when foreground application changes, switching is done by observing foreground application UID
     */
    IMPORT_C void SetLoadConfigByUid(TBool aLoadByUid);

    /*
     * CancelAll
     * Cancels all ongoing actions in server
     */
    IMPORT_C void CancelAll();
	
    /*
     * GetCurrentConfigName
     * Return: current xml key configuration config filename
     */
    IMPORT_C void GetCurrentConfigName(TFileName& aFilename);
    
private:
    TInt iBatteryStatus;
    TFileName iFilename;
    };

#endif
