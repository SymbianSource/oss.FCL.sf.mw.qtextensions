/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 2.1 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, 
 * see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
 *
 * Description:
 *
 */

#include "responsehandler.h"

#include <remconcoreapi.h>
#include <remconinterfaceselector.h>
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>    


ResponseHandler::ResponseHandler(
    CRemConCoreApiTarget& aRemConCoreApiTarget )
    : CActive( CActive::EPriorityStandard ),
      iRemConCoreApiTarget( aRemConCoreApiTarget )
    {
    CActiveScheduler::Add( this );
    }

ResponseHandler* ResponseHandler::NewL(
    CRemConCoreApiTarget& aRemConCoreApiTarget )
    {
    ResponseHandler* self =
        new (ELeave) ResponseHandler( aRemConCoreApiTarget );

    return self;
    }

ResponseHandler::~ResponseHandler()
    {
    Cancel();
    iResponseArray.Close();
    }

void ResponseHandler::CompleteAnyKey(TRemConCoreApiOperationId aOperationId)
    {
    if ( !IsActive() )
        {
/*
        switch ( aOperationId )
            {
            case ERemConCoreApiVolumeUp:
                {
                iRemConCoreApiTarget.VolumeUpResponse( iStatus, KErrNone );
                SetActive();
                break;
                }
            case ERemConCoreApiVolumeDown:
                {
                iRemConCoreApiTarget.VolumeDownResponse( iStatus, KErrNone );
                SetActive();
                break;
                }
            
            //TODO MORE!
                
            default:
                {
*/
                TInt error = KErrNone;
                iRemConCoreApiTarget.SendResponse(iStatus, aOperationId, error );
                SetActive();
/*
                break;
                }
            }
*/
        }
    // already active. Append to array and complete later.
    else
        {
        iResponseArray.Append( aOperationId );
        }
    }

void ResponseHandler::DoCancel()
    {
    }

void ResponseHandler::RunL()
    {
    // if any existing -> Send response
    if ( iResponseArray.Count() )
        {
        CompleteAnyKey( iResponseArray[0] );
        // Remove already completed key
        iResponseArray.Remove( 0 );
        iResponseArray.Compress();
        }
    }
