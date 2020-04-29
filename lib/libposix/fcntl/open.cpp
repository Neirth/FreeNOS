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
#include "errno.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"

int open(const char *path, int oflag, ...)
{
    FileSystemMessage msg;
    ProcessID mnt = findMount(path);
    FileDescriptor *files = getFiles();
    FileStat st;
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

    // Fill message
    msg.type   = ChannelMessage::Request;
    msg.action = StatFile;
    msg.path   = fullpath;
    msg.stat   = &st;

    // Ask the FileSystem for the file.
    if (mnt && files != NULL)
    {
        ChannelClient::instance->syncSendReceive(&msg, mnt);

        // Refresh mounts and retry, in case the file did not exist
        if (msg.result == ENOENT)
        {
            refreshMounts(0);
            mnt = findMount(path);
            msg.type = ChannelMessage::Request;
            ChannelClient::instance->syncSendReceive(&msg, mnt);
        }

        // Set errno
        errno = msg.result;

        if (msg.result == ESUCCESS)
        {
            // Insert into file descriptor table
            for (Size i = 0; i < FILE_DESCRIPTOR_MAX; i++)
            {
                if (!files[i].open)
                {
                    files[i].open  = true;
                    files[i].mount = mnt;
                    files[i].identifier = 0;
                    files[i].position = 0;
                    strlcpy(files[i].path, fullpath, PATH_MAX);
                    return i;
                }
            }
            // Too many open files
            errno = ENFILE;
        }
    }
    else
        errno = ENOENT;

    return -1;
}
