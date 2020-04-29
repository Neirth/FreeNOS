/*
 * Copyright (C) 2015 Niek Linnenbank
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
#include <errno.h>
#include "Runtime.h"
#include "unistd.h"

int unlink(const char *path)
{
    FileSystemMessage msg;
    ProcessID mnt = findMount(path);
    char fullpath[PATH_MAX];

    // Relative or absolute?
    if (path[0] != '/')
    {
        char cwd[PATH_MAX];

        // What's the current working dir?
        getcwd(cwd, PATH_MAX);
        snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, path);
    }
    else
        strlcpy(fullpath, path, sizeof(fullpath));

    // Ask for the unlink
    if (mnt)
    {
        msg.type   = ChannelMessage::Request;
        msg.action = DeleteFile;
        msg.path   = fullpath;
        msg.from   = SELF;
        ChannelClient::instance->syncSendReceive(&msg, mnt);

        // Set error number
        errno = msg.result;
    }
    else
        errno = ENOENT;

    // Done
    return errno == ESUCCESS ? 0 : (off_t) -1;
}
