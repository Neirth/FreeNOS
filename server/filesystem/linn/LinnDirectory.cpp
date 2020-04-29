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
#include "LinnDirectory.h"
#include "LinnFile.h"

LinnDirectory::LinnDirectory(LinnFileSystem *f,
                             LinnInode *i)
    : fs(f)
    , inode(i)
{
    m_size   = inode->size;
    m_access = inode->mode;
}

Error LinnDirectory::read(IOBuffer & buffer, Size size, Size offset)
{
    LinnSuperBlock *sb = fs->getSuperBlock();
    LinnDirectoryEntry dent;
    LinnInode *dInode;
    Size bytes = ZERO, blk;
    Error e;
    Dirent tmp;

    // Read directory entries
    for (u32 ent = 0; ent < inode->size / sizeof(LinnDirectoryEntry); ent++)
    {
        // Point to correct (direct) block
        if ((blk = (ent * sizeof(LinnDirectoryEntry)) / sb->blockSize)
             >= LINN_INODE_DIR_BLOCKS)
        {
            break;
        }
        // Calculate offset to read.
        u64 off = (inode->block[blk] * sb->blockSize) +
                      (ent * sizeof(LinnDirectoryEntry));

        // Get the next entry.
        if (fs->getStorage()->read(off, &dent,
                                   sizeof(LinnDirectoryEntry)) < 0)
        {
            return EACCES;
        }
        // Can we read another entry?
        if (bytes + sizeof(Dirent) > size)
        {
            return EFAULT;
        }
        // Fill in the Dirent.
        if (!(dInode = fs->getInode(dent.inode)))
        {
            return EINVAL;
        }
        strlcpy(tmp.name, dent.name, LINN_DIRENT_NAME_LEN);
        tmp.type = (FileType) dInode->type;

        // Copy to the buffer.
        if (( e = buffer.write(&tmp, sizeof(Dirent), bytes)) < 0)
        {
            return e;
        }
        bytes += sizeof(Dirent);
    }
    // All done.
    return bytes;
}

File * LinnDirectory::lookup(const char *name)
{
    LinnDirectoryEntry entry;
    LinnInode *inode;

    // Try to find the given LinnDirectoryEntry.
    if (!getLinnDirectoryEntry(&entry, name))
        return ZERO;

    // Then retrieve it's LinnInode.
    if (!(inode = fs->getInode(entry.inode)))
        return ZERO;

    // Create the appropriate in-memory file.
    switch ((FileType)inode->type)
    {
        case DirectoryFile:
            return new LinnDirectory(fs, inode);

        case RegularFile:
            return new LinnFile(fs, inode);

        default:
            return ZERO;
    }
}

bool LinnDirectory::getLinnDirectoryEntry(LinnDirectoryEntry *dent,
                                          const char *name)
{
    LinnSuperBlock *sb = fs->getSuperBlock();
    u64 offset;

    // Loop all blocks.
    for (u32 blk = 0; blk < LINN_INODE_NUM_BLOCKS(sb, inode); blk++)
    {
        // Read directory entries.
        for (u32 ent = 0; ent < LINN_DIRENT_PER_BLOCK(sb); ent++)
        {
            // Calculate offset to read.
            offset = (inode->block[blk] * sb->blockSize) +
                     (sizeof(LinnDirectoryEntry) * ent);

            // Get the next entry.
            if (fs->getStorage()->read(offset, dent,
                                       sizeof(LinnDirectoryEntry)) < 0)
            {
                return false;
            }
            // Is it the entry we are looking for?
            if (strcmp(name, dent->name) == 0)
            {
                return true;
            }
        }
    }
    // Not found.
    return false;
}
