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

#ifndef __FILESYSTEM_FILESYSTEM_H
#define __FILESYSTEM_FILESYSTEM_H

#include <FreeNOS/System.h>
#include <ChannelServer.h>
#include <Vector.h>
#include "Directory.h"
#include "Device.h"
#include "File.h"
#include "FileCache.h"
#include "FileSystemPath.h"
#include "FileSystemMessage.h"
#include "FileSystemRequest.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Abstract filesystem class.
 */
class FileSystem : public ChannelServer<FileSystem, FileSystemMessage>
{
  public:

    /**
     * Constructor function.
     *
     * @param p Path to which we are mounted.
     */
    FileSystem(const char *path);

    /**
     * Destructor function.
     */
    virtual ~FileSystem();

    /**
     * Get mount path
     *
     * @return Mount path of the filesystem
     */
    const char * getMountPath() const;

    /**
     * Get root directory.
     *
     * @return Root directory pointer
     */
    Directory * getRoot();

    /**
     * Mount the FileSystem.
     *
     * This function is responsible for mounting the
     * FileSystem.
     *
     * @return True if mounted successfully, false otherwise.
     */
    Error mount();

    /**
     * Register a new File.
     *
     * @param file File object pointer.
     * @param path The path for the File.
     *
     * @return Error code.
     */
    Error registerFile(File *file, const char *path, ...);

    /**
     * Register a new File with variable arguments.
     *
     * @param file File object pointer.
     * @param path The path for the File.
     * @param args Variable argument list.
     *
     * @return Error code.
     */
    Error registerFile(File *file, const char *path, va_list args);

    /**
     * Create a new file.
     *
     * @param type Describes the type of file to create.
     * @param deviceID Optionally specifies the device identities to create.
     *
     * @return Pointer to a new File on success or ZERO on failure.
     */
    virtual File * createFile(FileType type, DeviceID deviceID);

    /**
     * Inserts a file into the in-memory filesystem tree.
     *
     * @param file File to insert.
     * @param pathFormat Formatted full path to the file to insert.
     * @param ... Argument list.
     *
     * @return Pointer to the newly created FileCache, or NULL on failure.
     */
    FileCache * insertFileCache(File *file, const char *pathFormat, ...);

    /**
     * Inserts a file into the in-memory filesystem tree.
     *
     * @param file File to insert.
     * @param pathFormat Formatted full path to the file to insert.
     * @param args Argument list.
     *
     * @return Pointer to the newly created FileCache, or NULL on failure.
     */
    FileCache * insertFileCache(File *file, const char *pathFormat, va_list args);

    /**
     * Process an incoming filesystem request using a path.
     *
     * This message handler is responsible for processing any
     * kind of FileSystemMessages which have an FileSystemAction using
     * the path field, such as OpenFile.
     *
     * @param msg Incoming request message.
     * @see FileSystemMessage
     * @see FileSystemAction
     */
    void pathHandler(FileSystemMessage *msg);

    /**
     * Called when a sleep timeout is expired
     *
     * This function does a retry on all FileSystemRequests
     */
    virtual void timeout();

    /**
     * Retry any pending requests
     *
     * @return True if retry is needed again, false if all requests processed
     */
    virtual bool retryRequests();

  protected:

    /**
     * Process a FileSystemRequest.
     *
     * @return EAGAIN if the request cannot be completed yet or
     *         any other error code if processed.
     */
    Error processRequest(FileSystemRequest *req);

    /**
     * Send response for a FileSystemMessage
     *
     * @param msg The FileSystemMessage to send response for
     */
    void sendResponse(FileSystemMessage *msg);

    /**
     * Change the filesystem root directory.
     *
     * This function set the root member to the given
     * Directory pointer. Additionally, it inserts '/.' and '/..'
     * references to the file cache.
     *
     * @param newRoot A Directory pointer to set as the new root.
     *
     * @see root
     * @see insertFileCache
     */
    void setRoot(Directory *newRoot);

    /**
     * Retrieve a File from storage.
     *
     * This function is responsible for walking the
     * given FileSystemPath, retrieving each uncached File into
     * the FileCache, and returning a pointer to corresponding FileCache
     * of the last entry in the given path.
     *
     * @param path A path to lookup from storage.
     *
     * @return Pointer to a FileCache on success, ZERO otherwise.
     */
    FileCache * lookupFile(FileSystemPath *path);

    /**
     * Search the cache for an entry.
     *
     * @param path Full path of the file to find.
     *
     * @return Pointer to FileCache object on success, NULL on failure.
     */
    FileCache * findFileCache(char *path);

    /**
     * Search the cache for an entry.
     *
     * @param path Full path of the file to find.
     *
     * @return Pointer to FileCache object on success, NULL on failure.
     */
    FileCache * findFileCache(String *path);

    /**
     * Search the cache for an entry.
     *
     * @param path Full path of the file to find.
     *
     * @return Pointer to FileCache object on success, NULL on failure.
     */
    FileCache * findFileCache(FileSystemPath *p);

    /**
     * Process a cache hit.
     *
     * @param cache FileCache object which has just been referenced.
     *
     * @return FileCache object pointer.
     */
    virtual FileCache * cacheHit(FileCache *cache);

    /**
     * Cleans up the entire file cache (except opened file caches and root).
     *
     * @param cache Input FileCache object. ZERO to clean up all from root.
     */
    void clearFileCache(FileCache *cache = ZERO);

  protected:

    /** Root entry of the filesystem tree. */
    FileCache *m_root;

    /** Mount point. */
    const char *m_mountPath;

    /** Contains ongoing requests */
    List<FileSystemRequest *> *m_requests;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_FILESYSTEM_H */
