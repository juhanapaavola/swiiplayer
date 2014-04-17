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
#ifndef __BTCONTROLLERSERVER_H__
#define __BTCONTROLLERSERVER_H__

#include <e32base.h>
#include "btcontrollercommon.h"
#include "btcontroller.h"

class CBTController;

class CBTControllerServer: public CServer2
    {
public:
    static CServer2* NewLC();
    
    virtual ~CBTControllerServer();
    
    void AddSession();
    
    void DropSession();
    
private:
    CBTControllerServer();
    
    void ConstructL();

    TVersion Version();

    CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage ) const;

    void PanicServer( TInt aPanic );

private:
    CObjectConIx*   iSessionContainer;
    CBTController*  iController;
    TInt            iSessionCounter;    
    };

#endif


