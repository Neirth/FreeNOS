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
#include <ExecutableFormat.h>
#include <Types.h>
#include <Runtime.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "unistd.h"

int forkexec(const char *path, const char *argv[])
{
    ExecutableFormat *fmt;
    ExecutableFormat::Region regions[16];
    Memory::Range range;
    Arch::MemoryMap map;
    uint count = 0;
    pid_t pid = 0;
    Size numRegions = 16;
    int fd;
    struct stat st;
    u8 *image;
    Address entry;

    // Find program image
    if (stat(path, &st) != 0)
        return -1;

    // Open program image
    if ((fd = open(path, O_RDONLY)) < 0)
        return -1;

    // Read the program image
    image = new u8[st.st_size];
    if (read(fd, image, st.st_size) != st.st_size)
    {
        delete image;
        close(fd);
        return -1;
    }
    close(fd);

    // Attempt to read executable format
    if (ExecutableFormat::find(image, st.st_size, &fmt) != ExecutableFormat::Success)
    {
        delete image;
        errno = ENOEXEC;
        return -1;
    }

    // Retrieve entry point
    if (fmt->entry(&entry) != ExecutableFormat::Success)
    {
        delete fmt;
        delete image;
        errno = ENOEXEC;
        return -1;
    }

    // Create new process
    pid = ProcessCtl(ANY, Spawn, entry);
    if (pid == (pid_t) -1)
    {
        delete fmt;
        delete image;
        errno = EIO;
        return -1;
    }

    // Retrieve memory regions
    if (fmt->regions(regions, &numRegions) != ExecutableFormat::Success)
    {
        delete fmt;
        delete image;
        errno = ENOEXEC;
        return -1;
    }

    // Not needed anymore
    delete fmt;
    delete image;

    // Map program regions into virtual memory of the new process
    for (Size i = 0; i < numRegions; i++)
    {
        // Copy executable memory from this region
        range.virt   = regions[i].virt;
        range.phys   = ZERO;
        range.size   = regions[i].size;
        range.access = Memory::User |
                       Memory::Readable |
                       Memory::Writable |
                       Memory::Executable;

        // Create mapping first
        if (VMCtl(pid, Map, &range) != 0)
        {
            errno = EFAULT;
            return -1;
        }
        // Copy bytes
        VMCopy(pid, API::Write, (Address) regions[i].data,
               regions[i].virt, regions[i].size);

        // Release buffer
        delete regions[i].data;
    }

    // Create mapping for command-line arguments
    range = map.range(MemoryMap::UserArgs);
    range.phys = ZERO;
    range.access = Memory::User | Memory::Readable | Memory::Writable;
    VMCtl(pid, Map, &range);

    // Allocate arguments and current working directory
    char *arguments = new char[PAGESIZE*2];
    memset(arguments, 0, PAGESIZE);

    // Fill in arguments
    while (argv[count] && count < PAGESIZE / ARGV_SIZE)
    {
        strlcpy(arguments + (ARGV_SIZE * count), argv[count], ARGV_SIZE);
        count++;
    }

    // Fill in the current working directory
    strlcpy(arguments + PAGESIZE, **(getCurrentDirectory()), PATH_MAX);

    // Copy argc/argv into the new process
    if ((VMCopy(pid, API::Write, (Address) arguments, range.virt, PAGESIZE * 2)) < 0)
    {
        delete[] arguments;
        errno = EFAULT;
        ProcessCtl(pid, KillPID);
        return -1;
    }

    // Copy fds into the new process.
    if ((VMCopy(pid, API::Write, (Address) getFiles(),
                range.virt + (PAGESIZE * 2), range.size - (PAGESIZE * 2))) < 0)
    {
        delete[] arguments;
        errno = EFAULT;
        ProcessCtl(pid, KillPID);
        return -1;
    }

    // Let the Child begin execution
    ProcessCtl(pid, Resume);

    // Done. Cleanup.
    delete[] arguments;
    return pid;
}
