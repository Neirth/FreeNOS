/*
 * Copyright (C) 2019 Niek Linnenbank
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

#ifndef __FILESYSTEM_SYSINFOFILESYSTEM_H
#define __FILESYSTEM_SYSINFOFILESYSTEM_H

#include <FileSystem.h>
#include <FileSystemMessage.h>
#include <FileSystemPath.h>
#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup sysfs
 * @{
 */

/**
 * System information filesystem (SysFS).
 *
 * Provides generic information about the current hardware and software
 * of the running system. For example, SysFS provides the list of mounted filesystems.
 */
class SysInfoFileSystem : public FileSystem
{
  public:

    /**
     * Class constructor function.
     *
     * @param path Path to which we are mounted.
     */
    SysInfoFileSystem(const char *path);
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_SYSINFOFILESYSTEM_H */
