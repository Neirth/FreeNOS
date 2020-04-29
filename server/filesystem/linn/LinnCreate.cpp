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

#include <BitArray.h>
#include <List.h>
#include <ListIterator.h>
#include <String.h>
#include <Types.h>
#include <Macros.h>
#include <FileType.h>
#include <FileMode.h>
#include "LinnCreate.h"
#include "LinnSuperBlock.h"
#include "LinnGroup.h"
#include "LinnInode.h"
#include "LinnDirectoryEntry.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

LinnCreate::LinnCreate()
{
    prog      = ZERO;
    image     = ZERO;
    super     = ZERO;
    input     = ZERO;
    verbose   = false;
}

LinnInode * LinnCreate::createInode(le32 inodeNum, FileType type,
                                    FileModes mode, UserID uid, GroupID gid)
{
    LinnGroup *group;
    LinnInode *inode;
    BitArray inodeMap(super->inodesPerGroup);

    // Point to the correct group
    group  = BLOCKPTR(LinnGroup, super->groupsTable);
    group += inodeNum / super->inodesPerGroup;

    // Use it to find the inode
    inode  = BLOCKPTR(LinnInode, group->inodeTable);
    inode += inodeNum % super->inodesPerGroup;

    // Initialize the inode
    inode->type  = type;
    inode->mode  = mode;
    inode->uid   = uid;
    inode->gid   = gid;
    inode->size  = ZERO;
    inode->accessTime = ZERO;
    inode->createTime = time(ZERO);
    inode->modifyTime = inode->createTime;
    inode->changeTime = inode->createTime;
    inode->links = 1;

    // Update inode BitArray, if needed
    inodeMap.setArray(BLOCKPTR(u8, group->inodeMap),
                    super->inodesPerGroup);
    inodeMap.set(inodeNum % super->inodesPerGroup);

    // Update superblock
    super->freeInodesCount--;
    group->freeInodesCount--;

    // Done
    return inode;
}

le32 LinnCreate::createInode(char *inputFile, struct stat *st)
{
    LinnGroup *group;
    LinnInode *inode;
    BitArray inodeMap(super->inodesPerGroup);
    u32 gn, in;

    // Loop all available LinnGroups
    for (gn = 0; gn < LINN_GROUP_COUNT(super); gn++)
    {
        // Point to the correct LinnGroup
        group = BLOCKPTR(LinnGroup, super->groupsTable) + gn;

        // Does it have any free inodes?
        if (!group->freeInodesCount)
        {
            group = ZERO;
            continue;
        }
        else
            break;
    }

    // Did we find an appropriate group?
    if (!group)
    {
        printf("%s: no free inode available for `%s'\n",
                prog, inputFile);
        exit(EXIT_FAILURE);
    }

    // Find an empty inode number
    inodeMap.setArray(BLOCKPTR(u8, group->inodeMap),
                    super->inodesPerGroup);
    inodeMap.setNext(&in);

    // Instantiate the inode
    inode = createInode(in, FILETYPE_FROM_ST(st),
                            FILEMODE_FROM_ST(st), st->st_uid, st->st_gid);

    // Insert file contents
    if (S_ISREG(st->st_mode))
    {
        insertFile(inputFile, inode, st);
    }

    // Debug out
    if (verbose)
    {
        printf("%s inode=%u size=%u\n", inputFile, in, inode->size);
    }
    return in;
}

void LinnCreate::insertIndirect(le32 *ptr, le32 blockNumber,
                                le32 blockValue, Size depth)
{
    Size remain = 1;

    // Does the block map itself have a block?
    if (!*ptr)
    {
        *ptr = BLOCK(super);
    }

    // Point to the block map
    ptr = BLOCKPTR(u32, *ptr);

    // Calculate the number of blocks remaining per entry
    for (Size i = 0; i < depth - 1; i++)
    {
        remain *= LINN_SUPER_NUM_PTRS(super);
    }

    // More indirection?
    if (remain == 1)
    {
        ptr[blockNumber % LINN_SUPER_NUM_PTRS(super)] = blockValue;
    }
    // Traverse indirection
    else
    {
        insertIndirect(&ptr[blockNumber / remain],
                        blockNumber, blockValue, depth - 1);
    }
}

void LinnCreate::insertFile(char *inputFile, LinnInode *inode,
                            struct stat *st)
{
    int fd, bytes;
    le32 blockNr;

    // Open the local file
    if ((fd = open(inputFile, O_RDONLY)) < 0)
    {
        printf("%s: failed to fopen() `%s': %s\n",
                prog, inputFile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Read blocks from the file
    while (true)
    {
        // Grab a block
        blockNr = BLOCK(super);

        // Read a block
        if ((bytes = read(fd, BLOCKPTR(u8, blockNr), super->blockSize)) < 0)
        {
            printf("%s: failed to read() `%s': %s\n",
                    prog, inputFile, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // End of file?
        if (!bytes) break;

        // Insert the block (direct)
        if (LINN_INODE_NUM_BLOCKS(super, inode) <
            LINN_INODE_DIR_BLOCKS)
        {
            inode->block[LINN_INODE_NUM_BLOCKS(super,inode)] = blockNr;
        }
        // Insert the block (indirect)
        else if (LINN_INODE_NUM_BLOCKS(super, inode) <
                 LINN_INODE_DIR_BLOCKS + LINN_SUPER_NUM_PTRS(super))
        {
            insertIndirect(&inode->block[LINN_INODE_IND_BLOCKS-1],
                            LINN_INODE_NUM_BLOCKS(super, inode) -
                            LINN_INODE_DIR_BLOCKS, blockNr, 1);
        }
        // Insert the block (double indirect)
        else if (LINN_INODE_NUM_BLOCKS(super, inode) <
                 LINN_INODE_DIR_BLOCKS + (LINN_SUPER_NUM_PTRS(super) *
                                          LINN_SUPER_NUM_PTRS(super)))
        {
            insertIndirect(&inode->block[LINN_INODE_DIND_BLOCKS-1],
                            LINN_INODE_NUM_BLOCKS(super, inode) -
                            LINN_INODE_DIR_BLOCKS, blockNr, 2);
        }
        // Insert the blck (triple indirect)
        else if (LINN_INODE_NUM_BLOCKS(super, inode) <
                 LINN_INODE_DIR_BLOCKS + (LINN_SUPER_NUM_PTRS(super) *
                                          LINN_SUPER_NUM_PTRS(super) *
                                          LINN_SUPER_NUM_PTRS(super)))
        {
            insertIndirect(&inode->block[LINN_INODE_TIND_BLOCKS-1],
                            LINN_INODE_NUM_BLOCKS(super, inode) -
                            LINN_INODE_DIR_BLOCKS, blockNr, 3);
        }
        // Maximum file capacity reached
        else
        {
            printf("%s: maximum file size reached for `%s'\n",
                    prog, inputFile);
            break;
        }
        // Increment size appropriately
        inode->size += bytes;
    }
    // Cleanup
    close(fd);
}

void LinnCreate::insertEntry(le32 dirInode, le32 entryInode,
                             const char *name, FileType type)
{
    LinnGroup *group;
    LinnInode *inode;
    LinnDirectoryEntry *entry;
    le32 entryNum, blockNum;

    // Point to the correct group
    group = BLOCKPTR(LinnGroup, super->groupsTable);
    if (dirInode != ZERO)
    {
        group += (dirInode / super->inodesPerGroup);
    }
    // Fetch inode
    inode = BLOCKPTR(LinnInode, group->inodeTable) +
                    (dirInode % super->inodesPerGroup);

    // Calculate entry and block number
    entryNum = inode->size / sizeof(LinnDirectoryEntry);
    blockNum = (entryNum * sizeof(LinnDirectoryEntry)) /
                super->blockSize;

    // Direct block
    if (blockNum < LINN_INODE_DIR_BLOCKS)
    {
        // Allocate a new block, if needed
        if (!inode->block[blockNum])
        {
            inode->block[blockNum] = BLOCK(super);
        }
        // Point to the fresh entry
        entry = BLOCKPTR(LinnDirectoryEntry, inode->block[blockNum]) +
                        (entryNum % super->blockSize);
        // Fill it
        entry->inode = entryInode;
        entry->type  = type;
        strncpy(entry->name, name, LINN_DIRENT_NAME_LEN);
        entry->name[LINN_DIRENT_NAME_LEN - 1] = ZERO;
    }
    // Indirect block
    else
    {
        printf("%s: indirect blocks not (yet) supported for directories\n",
                prog);
        exit(EXIT_FAILURE);
    }
    // Increment directory size
    inode->size += sizeof(LinnDirectoryEntry);
}

void LinnCreate::insertDirectory(char *inputDir, le32 inodeNum, le32 parentNum)
{
    struct dirent *ent;
    struct stat st;
    DIR *dir;
    char path[255];
    le32 child;
    bool skip = false;

    // Create '.' and '..'
    insertEntry(inodeNum, inodeNum,  ".",  DirectoryFile);
    insertEntry(inodeNum, parentNum, "..", DirectoryFile);

    // Open the input directory
    if ((dir = opendir(inputDir)) == NULL)
    {
        printf("%s: failed to opendir() `%s': %s\n",
                prog, inputDir, strerror(errno));
        exit(EXIT_FAILURE);
    }
    // Read all it's entries
    while ((ent = readdir(dir)))
    {
        // Hidden files
        skip = ent->d_name[0] == '.';

        // Excluded files
        for (ListIterator<String *> e(&excludes); e.hasCurrent(); e++)
        {
            String dent = (const char *) ent->d_name;

            if (dent.match(**e.current()))
            {
                skip = true;
                break;
            }
        }
        // Skip file?
        if (skip) continue;

        // Construct local path
        snprintf(path, sizeof(path), "%s/%s",
                 inputDir, ent->d_name);

        // Stat the file
        if (stat(path, &st) != 0)
        {
            printf("%s: failed to stat() `%s': %s\n",
                    prog, path, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // Create an inode for the child
        child = createInode(path, &st);

        // Insert directory entry
        insertEntry(inodeNum, child, ent->d_name,
                    FILETYPE_FROM_ST(&st));

        // Traverse down
        if (S_ISDIR(st.st_mode))
        {
            insertDirectory(path, child, inodeNum);
        }
    }
    // All done
    closedir(dir);
}

int LinnCreate::create(Size blockSize, Size blockNum, Size inodeNum)
{
    LinnGroup *group;
    BitArray map(128);

    assert(image != ZERO);
    assert(prog  != ZERO);
    assert(blockNum >= 2);
    assert(inodeNum > 0);

    // Allocate blocks
    blocks = new u8[blockSize * blockNum];
    memset(blocks, 0, blockSize * blockNum);

    // Create a superblock
    super = (LinnSuperBlock *) (blocks + LINN_SUPER_OFFSET);
    super->magic0 = LINN_SUPER_MAGIC0;
    super->magic1 = LINN_SUPER_MAGIC1;
    super->majorRevision    = LINN_SUPER_MAJOR;
    super->minorRevision    = LINN_SUPER_MINOR;
    super->state            = LINN_SUPER_VALID;
    super->blockSize        = blockSize;
    super->blocksPerGroup   = LINN_CREATE_BLOCKS_PER_GROUP;
    super->inodesCount      = inodeNum;
    super->blocksCount            = blockNum;
    super->inodesPerGroup   = super->inodesCount / LINN_GROUP_COUNT(super);
    super->freeInodesCount  = super->inodesCount;
    super->freeBlocksCount  = blockNum - 3;
    super->creationTime     = time(ZERO);
    super->mountTime            = ZERO;
    super->mountCount            = ZERO;
    super->lastCheck            = ZERO;
    super->groupsTable            = 2;

    // Allocate LinnGroups
    for (Size i = 0; i < LINN_GROUP_COUNT(super); i++)
    {
        // Point to the correct LinnGroup
        group = BLOCKPTR(LinnGroup, 2) + i;

        // Fill the group
        group->freeBlocksCount = super->blocksPerGroup;
        group->freeInodesCount = super->inodesPerGroup;
        group->blockMap        = BLOCKS(super, LINN_GROUP_NUM_BLOCKMAP(super));
        group->inodeMap        = BLOCKS(super, LINN_GROUP_NUM_INODEMAP(super));
        group->inodeTable      = BLOCKS(super, LINN_GROUP_NUM_INODETAB(super));
    }

    // Create special inodes
    createInode(LINN_INODE_ROOT, DirectoryFile,
                OwnerRWX | GroupRX | OtherRX);
    createInode(LINN_INODE_LOADER, RegularFile,
                OwnerRWX | GroupRX | OtherRX);
    createInode(LINN_INODE_BAD, RegularFile,
                OwnerRW | GroupR | OtherR);
    createInode(LINN_INODE_JOURNAL, RegularFile,
                OwnerRW | GroupR | OtherR);

    // Insert into directory contents, if set
    if (input)
    {
        insertDirectory(input, LINN_INODE_ROOT,
                               LINN_INODE_ROOT);
    }
    // Mark blocks used
    for (le32 block = 0; block < super->freeBlocksCount; block++)
    {
        // Point to group
        group = BLOCKPTR(LinnGroup, super->groupsTable) +
                        (block / super->blocksPerGroup);
        group->freeBlocksCount--;

        // Mark the block used
        map.setArray(BLOCKPTR(u8, group->blockMap),
                   super->blocksPerGroup);
        map.set(block % super->blocksPerGroup);
    }
    // Write the final image
    return writeImage();
}

int LinnCreate::writeImage()
{
    FILE *fp;

    // Open output image
    if ((fp = fopen(image, "w")) == NULL)
    {
        printf("%s: failed to fopen() `%s' for writing: %s\r\n",
                prog, image, strerror(errno));
        return EXIT_FAILURE;
    }

    // Write all blocks at once
    if (fwrite(blocks, super->blockSize *
                      (super->blocksCount - super->freeBlocksCount), 1, fp) != 1)
    {
        printf("%s: failed to fwrite() `%s': %s\r\n",
                prog, image, strerror(errno));
        fclose(fp);
        return EXIT_FAILURE;
    }
    // All done
    fclose(fp);
    return EXIT_SUCCESS;
}

void LinnCreate::setProgram(char *progName)
{
    this->prog = progName;
}

void LinnCreate::setImage(char *imageName)
{
    this->image = imageName;
}

void LinnCreate::setInput(char *inputName)
{
    this->input = inputName;
}

void LinnCreate::setExclude(char *pattern)
{
    this->excludes.append(new String(pattern));
}

void LinnCreate::setVerbose(bool newVerbose)
{
    this->verbose = newVerbose;
}

int main(int argc, char **argv)
{
    LinnCreate fs;
    Size blockSize = LINN_CREATE_BLOCK_SIZE;
    Size blockNum  = LINN_CREATE_BLOCK_NUM;
    Size inodeNum  = LINN_CREATE_INODE_NUM;

    // Verify command-line arguments
    if (argc < 2)
    {
        printf("usage: %s IMAGE [OPTIONS...]\r\n"
               "Creates a new Linnenbank FileSystem\r\n"
               "\r\n"
               " -h           Show this help message.\r\n"
               " -v           Output verbose messages.\r\n"
               " -d DIRECTORY Insert files from the given directory into the image\r\n"
               " -e PATTERN   Exclude matching files from the created filesystem\r\n"
               " -b SIZE      Specifies the blocksize in bytes.\r\n"
               " -n COUNT     Specifies the maximum number of blocks.\r\n"
               " -i COUNT     Specifies the number of inodes to allocate.\r\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    // Process command-line arguments
    fs.setProgram(argv[0]);
    fs.setImage(argv[1]);

    // Process command-line options
    for (int i = 0; i < argc - 2; i++)
    {
        // Exclude files matching the given pattern
        if (!strcmp(argv[i + 2], "-e") && i < argc - 3)
        {
            fs.setExclude(argv[i + 3]);
            i++;
        }
        // Verbose output
        else if (!strcmp(argv[i + 2], "-v"))
        {
            fs.setVerbose(true);
        }
        // Input directory
        else if (!strcmp(argv[i + 2], "-d") && i < argc - 3)
        {
            fs.setInput(argv[i + 3]);
            i++;
        }
        // Block size
        else if (!strcmp(argv[i + 2], "-b") && i < argc - 3)
        {
            blockSize = atoi(argv[i + 3]);
            i++;
        }
        // Maximum block count
        else if (!strcmp(argv[i + 2], "-n") && i < argc - 3)
        {
            if ((blockNum = atoi(argv[i + 3])) < 2)
            {
                printf("%s: block count must be >= 2\r\n",
                        argv[0]);
                return EXIT_FAILURE;
            }
            i++;
        }
        // Inode count
        else if (!strcmp(argv[i + 2], "-i") && i < argc - 3)
        {
            if ((inodeNum = atoi(argv[i + 3])) < 1)
            {
                printf("%s: inode count must be >= 1\r\n",
                        argv[0]);
                return EXIT_FAILURE;
            };
            i++;
        }
        // Unknown argument
        else
        {
            printf("%s: unknown option `%s'\r\n",
                    argv[0], argv[i + 2]);
            return EXIT_FAILURE;
        }
    }
    // Create a new Linnenbank FileSystem
    return fs.create(blockSize, blockNum, inodeNum);
}
