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
#include <Types.h>
#include <Macros.h>
#include <Array.h>
#include <ChannelClient.h>
#include <PageAllocator.h>
#include <PoolAllocator.h>
#include <FileSystemMount.h>
#include <FileSystemMessage.h>
#include <MemoryMap.h>
#include <Core.h>
#include "FileDescriptor.h"
#include "stdlib.h"
#include "string.h"
#include "Runtime.h"
#include "unistd.h"
#include "libgen.h"
#include "fcntl.h"
#include "dirent.h"

/** List of constructors. */
extern void (*CTOR_LIST)();

/** List of destructors. */
extern void (*DTOR_LIST)();

/** FileSystem mounts table */
static FileSystemMount mounts[FILESYSTEM_MAXMOUNTS];

/** Table with FileDescriptors. */
static FileDescriptor *files = (FileDescriptor *) NULL;

/** Current Directory String */
String *currentDirectory = (String *) NULL;

void * __dso_handle = 0;

extern C void __aeabi_unwind_cpp_pr0()
{
}

extern C int __cxa_atexit(void (*func) (void *),
                          void * arg, void * dso_handle)
{
    return (0);
}

extern C int __aeabi_atexit()
{
    return 0;
}

extern C void __cxa_pure_virtual()
{
}

extern C void __stack_chk_fail(void)
{
}

extern C int raise(int sig)
{
    return 0;
}

void runConstructors()
{
    for (void (**ctor)() = &CTOR_LIST; ctor && *ctor; ctor++)
    {
        (*ctor)();
    }
}

void runDestructors()
{
    for (void (**dtor)() = &DTOR_LIST; dtor && *dtor; dtor++)
    {
        (*dtor)();
    }
}

void setupHeap()
{
    PageAllocator *pageAlloc;
    PoolAllocator *poolAlloc;
    Arch::MemoryMap map;
    Memory::Range heap = map.range(MemoryMap::UserHeap);

    // Allocate one page to store the allocators themselves
    Memory::Range range;
    range.size   = PAGESIZE;
    range.access = Memory::User | Memory::Readable | Memory::Writable;
    range.virt   = heap.virt;
    range.phys   = ZERO;
    VMCtl(SELF, Map, &range);

    // Allocate instance copy on vm pages itself
    pageAlloc = new (heap.virt) PageAllocator(heap.virt, heap.size);
    poolAlloc = new (heap.virt + sizeof(PageAllocator)) PoolAllocator();
    poolAlloc->setParent(pageAlloc);

    // Set default allocator
    Allocator::setDefault(poolAlloc);
}

void setupRandomizer()
{
    ProcessID pid = getpid();
    Timer::Info timer;
    ProcessCtl(SELF, InfoTimer, (Address) &timer);

    ::srandom(pid + timer.ticks);
}

void setupChannels()
{
    ChannelClient *client = new ChannelClient();
    (void) client;

    ChannelClient::instance->setRegistry(new ChannelRegistry());
}

void setupMappings()
{
    // Fill the mounts table
    memset(mounts, 0, sizeof(FileSystemMount) * FILESYSTEM_MAXMOUNTS);
    strlcpy(mounts[0].path, "/sys", PATH_MAX);
    strlcpy(mounts[1].path, "/", PATH_MAX);
    mounts[0].procID  = SYSFS_PID;
    mounts[0].options = ZERO;
    mounts[1].procID  = ROOTFS_PID;
    mounts[1].options = ZERO;

    // Map user program arguments
    Arch::MemoryMap map;
    Memory::Range argRange = map.range(MemoryMap::UserArgs);

    // First page is the argc+argv (skip here)
    // Second page is the current working directory
    currentDirectory = new String((char *) argRange.virt + PAGESIZE, false);

    // Third page and above contain the file descriptors table
    files = (FileDescriptor *) (argRange.virt + (PAGESIZE * 2));

    // Inherit file descriptors table from parent (if any).
    // Without a parent, just clear the file descriptors
    if (getppid() == 0)
    {
        memset(files, 0, argRange.size - (PAGESIZE * 2));
        (*currentDirectory) = "/";
    }
}

ProcessID findMount(const char *path)
{
    FileSystemMount *m = ZERO;
    Size length = 0, len;
    char tmp[PATH_MAX];

    // Is the path relative?
    if (path[0] != '/')
    {
        getcwd(tmp, sizeof(tmp));
        snprintf(tmp, sizeof(tmp), "%s/%s", tmp, path);
    }
    else
        strlcpy(tmp, path, PATH_MAX);

    // Find the longest match
    for (Size i = 0; i < FILESYSTEM_MAXMOUNTS; i++)
    {
        if (mounts[i].path[0])
        {
            len = strlen(mounts[i].path);

            // Only choose this mount, if it matches,
            // and is longer than the last match.
            if (strncmp(tmp, mounts[i].path, len) == 0 && len > length)
            {
                length = len;
                m = &mounts[i];
            }
        }
    }

    // All done
    return m ? m->procID : ZERO;
}

void refreshMounts(const char *path)
{
    FileSystemMessage msg;
    pid_t pid = getpid();

    // Skip for rootfs and sysfs
    if (pid == ROOTFS_PID || pid == SYSFS_PID)
        return;

    // Clear mounts table
    MemoryBlock::set(&mounts[2], 0, sizeof(FileSystemMount) * (FILESYSTEM_MAXMOUNTS-2));

    // Re-read the mounts table from SysFS.
    msg.type   = ChannelMessage::Request;
    msg.action = ReadFile;
    msg.path   = "/sys/mounts";
    msg.buffer = (char *) &mounts;
    msg.size   = sizeof(FileSystemMount) * FILESYSTEM_MAXMOUNTS;
    msg.offset = 0;
    msg.from   = SELF;
    ChannelClient::instance->syncSendReceive(&msg, SYSFS_PID);
}

ProcessID findMount(int fildes)
{
    if (files != NULL)
        return files[fildes].open ? files[fildes].mount : ZERO;
    else
        return ZERO;
}

void waitMount(const char *path)
{
    FileSystemMessage msg;

    // Send a write containing the requested path to the 'mountwait' file on SysFS
    msg.type   = ChannelMessage::Request;
    msg.action = WriteFile;
    msg.path   = "/sys/mountwait";
    msg.buffer = (char *) path;
    msg.size   = strlen(path);
    msg.offset = 0;
    msg.from   = SELF;
    ChannelClient::instance->syncSendReceive(&msg, SYSFS_PID);
}

FileSystemMount * getMounts()
{
    return mounts;
}

FileDescriptor * getFiles(void)
{
    return files;
}

String * getCurrentDirectory()
{
    return currentDirectory;
}

extern C void SECTION(".entry") _entry()
{
    int ret, argc;
    char *arguments;
    char **argv;
    SystemInformation info;
    Arch::MemoryMap map;

    // Clear BSS
    clearBSS();

    // Setup the heap, C++ constructors and default mounts
    setupHeap();
    runConstructors();
    setupChannels();
    setupMappings();
    setupRandomizer();

    // Allocate buffer for arguments
    argc = 0;
    argv = (char **) new char[ARGV_COUNT];
    arguments = (char *) map.range(MemoryMap::UserArgs).virt;

    // Fill in arguments list
    while (argc < ARGV_COUNT && *arguments)
    {
        argv[argc] = arguments;
        arguments += ARGV_SIZE;
        argc++;
    }

    // Pass control to the program
    ret = main(argc, argv);

    // Terminate execution
    runDestructors();
    exit(ret);
}
