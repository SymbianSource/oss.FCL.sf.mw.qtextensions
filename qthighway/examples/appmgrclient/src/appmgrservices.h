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

#ifndef APPMGR_SERVICES_H
#define APPMGR_SERVICES_H

#include <xqaiwdecl.h>

// All the services should be declared in a component specific header file
// In order to avoid any mistakes in service naming.
// In QtHighway service interface is described in XML file, operation is not

// Service interfaces and related operations
#define SERVICE1 QLatin1String("serviceapp")
#define IDIAL QLatin1String("dialer")
#define OPERATION1 QLatin1String("dial(QString,bool)")

// Erroneus services
#define ERR_IDIAL QLatin1String("xxxx.yyy")
#define ERR_OPERATION1 QLatin1String("dial(QString,QString)")


// QtHighway specific:

#endif
