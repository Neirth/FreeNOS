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

#ifndef __FILESYSTEM_FILE_SYSTEM_MESSAGE_H
#define __FILESYSTEM_FILE_SYSTEM_MESSAGE_H

#include <FreeNOS/System.h>
#include <Types.h>
#include "ChannelMessage.h"
#include "FileType.h"
#include "FileMode.h"
#include "FileStat.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Actions which may be performed on the filesystem.
 */
typedef enum FileSystemAction
{
    CreateFile = 0,
    ReadFile,
    WriteFile,
    StatFile,
    DeleteFile
}
FileSystemAction;

/**
 * FileSystem IPC message.
 */
typedef struct FileSystemMessage : public ChannelMessage
{
    /**
     * Assignment operator.
     * @param m FileSystemMessage pointer to copy from.
     */
    void operator = (FileSystemMessage *m)
    {
        type        = m->type;
        identifier  = m->identifier;
        from        = m->from;
        action      = m->action;
        result      = m->result;
        buffer      = m->buffer;
        size        = m->size;
        offset      = m->offset;
        userID      = m->userID;
        groupID     = m->groupID;
        deviceID    = m->deviceID;
        mode        = m->mode;
        stat        = m->stat;
        path        = m->path;
        filetype    = m->filetype;
    }

    /**
     * Comparison operator.
     *
     * @param m Other FileSystemMessage to compare with.
     *
     * @return True if equal, false otherwise.
     */
    bool operator == (FileSystemMessage *m)
    {
        return this->from   == m->from &&
               this->action == m->action;
    }

    /** Source of the message */
    ProcessID from;

    /** Action to perform. */
    FileSystemAction action;

    /** Result code. */
    Error result;

    /** Points to a buffer for I/O. */
    char *buffer;

    /** Size of the buffer. */
    Size size;

    /** Offset in the file for I/O. */
    Size offset;

    /** Path name of the file. */
    char *path;

    /** User ID and group ID. */
    u16 userID, groupID;

    /** Filetype. */
    FileType filetype;

    /** File mode permissions. */
    FileModes mode;

    /** File Statistics. */
    FileStat *stat;

    /** Device major/minor numbers. */
    DeviceID deviceID;
}
FileSystemMessage;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILE_SYSTEM_MESSAGE_H */
