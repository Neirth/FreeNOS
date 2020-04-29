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

#ifndef __FILESYSTEM_TMPFILESYSTEM_H
#define __FILESYSTEM_TMPFILESYSTEM_H

#include <FileSystem.h>
#include <FileSystemMessage.h>
#include <FileSystemPath.h>
#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup tmpfs
 * @{
 */

/**
 * Temporary filesystem (TmpFS). Maps files into virtual memory.
 */
class TmpFileSystem : public FileSystem
{
  public:

    /**
     * Class constructor function.
     *
     * @param path Path to which we are mounted.
     */
    TmpFileSystem(const char *path);

    /**
     * @brief Creates a new TmpFile.
     *
     * @param path Full path to the file to create.
     * @param type Describes the file type to create.
     * @param deviceID Optionally specifies which device identities to use.
     *
     * @return Pointer to a new File on success, or ZERO on failure.
     *
     * @see File
     * @see FileSystemPath
     */
    virtual File * createFile(FileType type, DeviceID deviceID);
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_TMPFILESYSTEM_H */
