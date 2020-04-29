/*
 * Copyright (C) 2009 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FreeNOS/System.h>
#include <FileSystemMessage.h>
#include "Runtime.h"
#include <errno.h>
#include "unistd.h"

ssize_t write(int fildes, const void *buf, size_t nbyte)
{
    FileSystemMessage msg;
    FileDescriptor *files = getFiles();

    if (fildes >= FILE_DESCRIPTOR_MAX || fildes < 0)
    {
        errno = ERANGE;
        return -1;
    }

    // Do we have this file descriptor?
    if (!files[fildes].open)
    {
        errno = ENOENT;
        return -1;
    }

    // Write the file
    msg.type   = ChannelMessage::Request;
    msg.action = WriteFile;
    msg.path   = files[fildes].path;
    msg.buffer = (char *) buf;
    msg.size   = nbyte;
    msg.offset = files[fildes].position;
    msg.from   = SELF;
    msg.deviceID.minor = files[fildes].identifier;
    ChannelClient::instance->syncSendReceive(&msg, files[fildes].mount);

    // Did we write something?
    if (msg.result >= 0)
    {
        files[fildes].position += msg.result;
        return msg.result;
    }

    // Set error number
    errno = msg.result;
    return -1;
}
