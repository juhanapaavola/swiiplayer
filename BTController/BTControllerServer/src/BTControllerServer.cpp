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
#include "BTControllerServer.h"
#include "btcontrollerserversession.h"
#include "BTControllerCommon.h"

static void RunServerL()
    {
    CActiveScheduler* s = new(ELeave) CActiveScheduler;
    CleanupStack::PushL( s );
    CActiveScheduler::Install( s );
  
    CServer2* server = CBTControllerServer::NewLC();

    User::LeaveIfError( User::RenameThread( KServerName ) );
    User::SetCritical( User::EProcessCritical );
  
    RProcess::Rendezvous( KErrNone );

    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(server);
    CleanupStack::PopAndDestroy(s);
    }

TInt E32Main()
    {
    __UHEAP_MARK;
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt r( KErrNoMemory );
    if (cleanup)
      {
      TRAP( r, RunServerL() );
      delete cleanup;
      }
    __UHEAP_MARKEND;
    return r;
    }

CServer2* CBTControllerServer::NewLC()
    {
    CBTControllerServer* self = new ( ELeave ) CBTControllerServer();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }

void CBTControllerServer::AddSession()
    {
    iSessionCounter++;
    }

void CBTControllerServer::DropSession()
    {
    if ( iSessionCounter > 0 )
        {
        iSessionCounter--;
        }
    if(iSessionCounter == 0)
        {
        CActiveScheduler::Stop();
        }
    }


CBTControllerServer::CBTControllerServer():
CServer2(EPriorityStandard, ESharableSessions),
iSessionCounter(0)
    {
    
    }

CBTControllerServer::~CBTControllerServer()
    {
    if( iSessionContainer )
        {
        delete iSessionContainer;
        iSessionContainer = NULL;
        }
    if( iController )
        {
        delete iController;
        iController = NULL;
        }
    }

void CBTControllerServer::ConstructL()
    {
    StartL(KServerName);
    iSessionContainer = CObjectConIx::NewL();
    iController = CBTController::NewL();
    }

TVersion CBTControllerServer::Version()
    {
    return TVersion( KServerMajorVersionNumber, KServerMinorVersionNumber, KServerBuildVersionNumber );
    }

CSession2* CBTControllerServer::NewSessionL(const TVersion& aVersion, const RMessage2& /*aMessage*/ ) const
    {
    if ( !User::QueryVersionSupported( TVersion( KServerMajorVersionNumber, KServerMinorVersionNumber, KServerBuildVersionNumber ), aVersion ) )
            {
            User::Leave( KErrNotSupported );
            }
    return CBTControllerServerSession::NewL( *iSessionContainer, *iController, (CBTControllerServer *)this );
    }
                        

void CBTControllerServer::PanicServer( TInt aPanic )
    {
    User::Panic( KServerName, aPanic );
    }

