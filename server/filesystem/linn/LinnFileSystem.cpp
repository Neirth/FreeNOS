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

#include <Types.h>
#include <KernelLog.h>
#include <FileStorage.h>
#include <BootImageStorage.h>
#include "LinnFileSystem.h"
#include "LinnInode.h"
#include "LinnFile.h"
#include "LinnDirectory.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    KernelLog log;
    Storage *storage = ZERO;
    const char *path = "/";
    SystemInformation info;

    // Only run on core0
    if (info.coreId != 0)
        return EXIT_SUCCESS;

    log.setMinimumLogLevel(Log::Notice);

    // Mount the given file, or try to use the BootImage embedded rootfs
    if (argc > 3)
    {
        NOTICE("file storage: " << argv[1] << " at offset " << atoi(argv[2]));
        storage    = new FileStorage(argv[1], atoi(argv[2]));
        path       = argv[3];
    }
    else
    {
        BootImageStorage *bm = new BootImageStorage(LINNFS_ROOTFS_FILE);
        if (bm->load())
        {
            NOTICE("boot image: " << LINNFS_ROOTFS_FILE);
            storage = bm;
        } else
            FATAL("unable to load: " << LINNFS_ROOTFS_FILE);
    }

    // Mount, then start serving requests.
    if (storage)
    {
        LinnFileSystem server(path, storage);
        server.mount();
        return server.run();
    }
    ERROR("no usable storage found");
    return EXIT_FAILURE;
}

LinnFileSystem::LinnFileSystem(const char *p, Storage *s)
    : FileSystem(p), storage(s), groups(ZERO)
{
    LinnInode *rootInode;
    LinnGroup *group;
    Size offset;
    Error e;

    // Read out the superblock.
    if ((e = s->read(LINN_SUPER_OFFSET, &super,
                     sizeof(super))) <= 0)
    {
        FATAL("reading superblock failed: " <<
               strerror(e));
    }
    // Verify magic.
    if (super.magic0 != LINN_SUPER_MAGIC0 ||
        super.magic1 != LINN_SUPER_MAGIC1)
    {
        FATAL("magic mismatch");
    }
    // Create groups vector.
    groups = new Vector<LinnGroup *>(LINN_GROUP_COUNT(&super));
    groups->fill(ZERO);

    // Read out group descriptors.
    for (Size i = 0; i < LINN_GROUP_COUNT(&super); i++)
    {
        // Allocate buffer.
        group  = new LinnGroup;
        offset = (super.groupsTable * super.blockSize) +
                 (sizeof(LinnGroup)  * i);

        // Read from storage.
        if ((e = s->read(offset, group, sizeof(LinnGroup))) <= 0)
        {
            FATAL("reading group descriptor failed: " <<
                   strerror(e));
        }
        // Insert in the groups vector.
        groups->insert(i, group);
    }
    // Print out superblock information.

    INFO(LINN_GROUP_COUNT(&super) << " group descriptors");
    INFO(super.inodesCount - super.freeInodesCount << " inodes, " <<
         super.blocksCount - super.freeBlocksCount << " blocks");

    // Read out the root directory.
    rootInode = getInode(LINN_INODE_ROOT);
    setRoot(new LinnDirectory(this, rootInode));

    // Filesystem writes are not supported
    addIPCHandler(CreateFile, (IPCHandlerFunction) &LinnFileSystem::notSupportedHandler, false);
    addIPCHandler(DeleteFile, (IPCHandlerFunction) &LinnFileSystem::notSupportedHandler, false);
    addIPCHandler(WriteFile,  (IPCHandlerFunction) &LinnFileSystem::notSupportedHandler, false);

    // Done.
    NOTICE("mounted at " << p);
}

LinnInode * LinnFileSystem::getInode(u32 inodeNum)
{
    LinnGroup *group;
    LinnInode *inode;
    Size offset;
    Error e;

    // Validate the inode number.
    if (inodeNum >= super.inodesCount)
    {
        return ZERO;
    }
    // Do we have this Inode cached already?
    if (inodes.contains(inodeNum))
    {
        return inodes.value(inodeNum);
    }
    // Get the group descriptor.
    if (!(group = getGroupByInode(inodeNum)))
    {
        return ZERO;
    }
    // Allocate inode buffer.
    inode  = new LinnInode;
    offset = (group->inodeTable * super.blockSize) +
                ((inodeNum % super.inodesPerGroup) * sizeof(LinnInode));

    // Read inode from storage.
    if ((e = storage->read(offset, inode, sizeof(LinnInode))) <= 0)
    {
        ERROR("reading inode failed: " <<
               strerror(e));
        return ZERO;
    }
    // Insert into the cache.
    inodes.insert(inodeNum, inode);
    return inode;
}

LinnGroup * LinnFileSystem::getGroup(u32 groupNum)
{
    return (*groups)[groupNum];
}

LinnGroup * LinnFileSystem::getGroupByInode(u32 inodeNum)
{
    return getGroup(inodeNum ? inodeNum / super.inodesPerGroup : 0);
}

u64 LinnFileSystem::getOffset(LinnInode *inode, u32 blk)
{
    u64 numPerBlock = LINN_SUPER_NUM_PTRS(&super), offset;
    u32 *block = ZERO;
    Size depth = ZERO, remain = 1;

    // Direct blocks.
    if (blk < LINN_INODE_DIR_BLOCKS)
    {
        return inode->block[blk] * super.blockSize;
    }
    // Indirect blocks.
    if (blk - LINN_INODE_DIR_BLOCKS < numPerBlock)
    {
        depth = 1;
    }
    // Double indirect blocks.
    else if (blk - LINN_INODE_DIR_BLOCKS < numPerBlock * numPerBlock)
    {
        depth = 2;
    }
    // Triple indirect blocks.
    else
        depth = 3;

    // Allocate temporary block.
    block   = new u32[LINN_SUPER_NUM_PTRS(&super)];
    offset  = inode->block[(LINN_INODE_DIR_BLOCKS + depth - 1)];
    offset *= super.blockSize;

    // Lookup the block number.
    while (true)
    {
        // Fetch block.
        if (storage->read(offset, block, super.blockSize) < 0)
        {
            delete block;
            return 0;
        }
        // Calculate the number of blocks remaining per entry.
        for (Size i = 0; i < depth - 1; i++)
        {
            remain *= LINN_SUPER_NUM_PTRS(&super);
        }
        // More indirection?
        if (remain == 1)
        {
            break;
        }
        // Calculate the next offset.
        offset  = block[ (blk - LINN_INODE_DIR_BLOCKS) / remain ];
        offset *= super.blockSize;
        remain  = 1;
        depth--;
    }
    // Calculate the final offset.
    offset  = block[ (blk - LINN_INODE_DIR_BLOCKS) %
                      LINN_SUPER_NUM_PTRS(&super) ];
    offset *= super.blockSize;

    // All done.
    delete block;
    return offset;
}

void LinnFileSystem::notSupportedHandler(FileSystemMessage *msg)
{
    msg->result = ENOTSUP;
    sendResponse(msg);
}
