/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "v4lvideobuffer.h"

#include <linux/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>


V4LVideoBuffer::V4LVideoBuffer(unsigned char *buffer, int fd, v4l2_buffer buf)
    : QAbstractVideoBuffer(NoHandle)
    , m_buffer(buffer)
    , m_length(buf.bytesused)
    , m_fd(fd)
    , m_bytesPerLine(0)
    , m_mode(NotMapped)
    , m_buf(buf)
{
}

V4LVideoBuffer::~V4LVideoBuffer()
{
    ioctl(m_fd, VIDIOC_QBUF, &m_buf);
}

void V4LVideoBuffer::setBytesPerLine(int bytesPerLine)
{
    m_bytesPerLine = bytesPerLine;
}

QAbstractVideoBuffer::MapMode V4LVideoBuffer::mapMode() const
{
    return m_mode;
}

uchar *V4LVideoBuffer::map(MapMode mode, int *numBytes, int *bytesPerLine)
{
    if (mode != NotMapped && m_mode == NotMapped) {
        if (numBytes)
            *numBytes = m_length;

        m_mode = mode;

        if(bytesPerLine)
            *bytesPerLine = m_bytesPerLine;

        return m_buffer;
    } else {
        return 0;
    }
}
void V4LVideoBuffer::unmap()
{
    m_mode = NotMapped;
}

