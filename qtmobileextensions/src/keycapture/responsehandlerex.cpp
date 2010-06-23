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

#include "ResponseHandlerEx.h"

#include <remconcoreapi.h>
#include <remconinterfaceselector.h>
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>

#include <remconcallhandlingtarget.h>

ResponseHandlerEx::ResponseHandlerEx(
        CRemConCallHandlingTarget& aRemConCallHandlingTarget) :
    CActive(CActive::EPriorityStandard), iRemConCallHandlingTarget(
            aRemConCallHandlingTarget)
{
    CActiveScheduler::Add(this);
}

ResponseHandlerEx* ResponseHandlerEx::NewL(
        CRemConCallHandlingTarget& aRemConCallHandlingTarget)
{
    ResponseHandlerEx* self = new (ELeave) ResponseHandlerEx(
            aRemConCallHandlingTarget);

    return self;
}

ResponseHandlerEx::~ResponseHandlerEx()
{
    Cancel();
    iResponseArray.Close();
}

void ResponseHandlerEx::CompleteAnyKey(int id)
{
    if (!IsActive())
        {
        TInt error = KErrNone;
        iRemConCallHandlingTarget.SendResponse(iStatus, id, error);
        SetActive();
        }
    else
        {
        // already active. Append to array and complete later.
        iResponseArray.Append(id);
        }
}

void ResponseHandlerEx::DoCancel()
{
}

void ResponseHandlerEx::RunL()
{
    // if any existing -> Send response
    if (iResponseArray.Count())
        {
        CompleteAnyKey(iResponseArray[0]);
        // Remove already completed key
        iResponseArray.Remove(0);
        iResponseArray.Compress();
        }
}
