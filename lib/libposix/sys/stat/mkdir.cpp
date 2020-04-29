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
#include "sys/stat.h"

int mkdir(const char *path, mode_t mode)
{
    FileSystemMessage msg;
    ProcessID mnt = findMount(path);

    // Fill message
    msg.type   = ChannelMessage::Request;
    msg.action = CreateFile;
    msg.path   = (char *) path;
    msg.mode   = mode;
    msg.filetype = DirectoryFile;

    // Ask the FileSystem to create it
    if (mnt)
    {
        ChannelClient::instance->syncSendReceive(&msg, mnt);

        // Set errno
        errno = msg.result;
    }
    else
        errno = ENOENT;

    // Success
    return msg.result == ESUCCESS ? 0 : -1;
}
