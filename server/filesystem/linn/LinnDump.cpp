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
#include "LinnSuperBlock.h"
#include "LinnGroup.h"
#include "LinnInode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

char * timeString(u32 timestamp)
{
    char *buff = (char *) malloc(128);
    time_t timeval = (time_t) timestamp;
    struct tm *tm;

    // Check for zero timestamps.
    if (!timestamp)
    {
        return strdup("Never");
    }
    // Fill in the time struct.
    tm = gmtime(&timeval);

    // Format time.
    if (!strftime(buff, 128, "%F %T", tm))
    {
        return strdup("???");
    }
    // Done.
    else
        return buff;
}

void usage(char *prog)
{
    printf("usage: %s FILE [OPTIONS...]\r\n"
           "Displays information of a Linnenbank Filesystem\r\n"
           "\r\n"
           "-h          Show this help message.\r\n",
            prog);
}

int main(int argc, char **argv)
{
    LinnSuperBlock super;
    LinnGroup group;
    float percentFreeBlocks = 0, percentFreeInodes = 0, megabytes = 0;
    FILE *fp;

    // Verify command-line arguments.
    if (argc < 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    // Process command-line options.
    for (int i = 0; i < argc - 2; i++)
    {
        // Show help.
        if (!strcmp(argv[i + 2], "-h"))
        {
            usage(argv[0]);
            return EXIT_SUCCESS;
        }
        // Unknown argument.
        else
            printf("%s: unknown option `%s'\r\n",
                    argv[0], argv[i + 2]);
    }
    // Attempt to open the given file.
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("%s: failed to fopen() `%s': %s\n",
                argv[0], argv[1], strerror(errno));
        return EXIT_FAILURE;
    }
    // Seek to correct offset.
    if (fseek(fp, LINN_SUPER_OFFSET, SEEK_SET) == -1)
    {
        printf("%s: failed to fseek() to %x in `%s': %s\n",
                argv[0], LINN_SUPER_OFFSET, argv[1], strerror(errno));
        return EXIT_FAILURE;
    }
    // Read the superblock.
    if (fread(&super, sizeof(super), 1, fp) != 1)
    {
        printf("%s: failed to fread() superblock from `%s': %s\n",
                argv[0], argv[1], ferror(fp) ? strerror(errno) : "End of file");
        return EXIT_FAILURE;
    }
    // Verify magic.
    if (super.magic0 != LINN_SUPER_MAGIC0 ||
        super.magic1 != LINN_SUPER_MAGIC1)
    {
        printf("%s: `%s' is not a LinnFS filesystem (magic mismatch)\n",
                argv[0], argv[1]);
        return EXIT_FAILURE;
    }
    // Calculate the percentage of free blocks.
    if (super.blocksCount)
        percentFreeBlocks = ((float) super.freeBlocksCount /
                             (float) super.blocksCount) * 100.0;

    // Percentage of free inodes.
    if (super.inodesCount)
        percentFreeInodes = ((float) super.freeInodesCount /
                             (float) super.inodesCount) * 100.0;

    // Maximum number of megabytes the filesystem can manage.
    megabytes = (float) super.blocksCount * (float) super.blockSize /
                (1024.0 * 1024.0);

    // Dump superblock information.
    printf( "LinnSuperBlock\n"
            "[\n"
            "   magic0          = %x\n"
            "   magic1          = %x\n"
            "   majorRevision   = %u\n"
            "   minorRevision   = %u\n"
            "   state           = %x\n"
            "   blockSize       = %u\n"
            "   blocksPerGroup  = %u\n"
            "   inodesPerGroup  = %u\n"
            "   inodesCount     = %u\n"
            "   blocksCount     = %u (%.2fMB)\n"
            "   freeBlocksCount = %u (%.2f%%)\n"
            "   freeInodesCount = %u (%.2f%%)\n"
            "   creationTime    = %s\n"
            "   mountTime       = %s\n"
            "   mountCount      = %u\n"
            "   lastCheck       = %s\n"
            "   groupsTable     = %u\n"
            "]\n",
            super.magic0, super.magic1,
            super.majorRevision, super.minorRevision,
            super.state,  super.blockSize,
            super.blocksPerGroup, super.inodesPerGroup, super.inodesCount,
            super.blocksCount, megabytes,
            super.freeBlocksCount, percentFreeBlocks,
            super.freeInodesCount, percentFreeInodes,
            timeString(super.creationTime),
            timeString(super.mountTime), super.mountCount,
            timeString(super.lastCheck), super.groupsTable);

    // Seek to the group table.
    if (fseek(fp, super.groupsTable * super.blockSize, SEEK_SET) == -1)
    {
        printf("%s: failed to seek to LinnGroup table in `%s': %s\n",
                argv[0], argv[1], strerror(errno));
        return EXIT_FAILURE;
    }
    // Dump group information.
    for (Size i = 0; i < LINN_GROUP_COUNT(&super); i++)
    {
        // Read the LinnGroup.
        if (fread(&group, sizeof(group), 1, fp) != 1)
        {
            printf("%s: failed to fread() group from `%s': %s\n",
                        argv[0], argv[1], ferror(fp) ? strerror(errno) : "End of file");
            return EXIT_FAILURE;
        }
        // Dump group.
        printf(        "LinnGroup #%u (blocks %u - %u)\n"
                "[\n"
                "   freeBlocksCount = %u\n"
                "   freeInodesCount = %u\n"
                "   blockMap        = %u - %lu\n"
                "   inodeMap        = %u - %lu\n"
                "   inodeTable      = %u - %lu\n"
                "]\n",
                i,
                i * super.blocksPerGroup,
                (i + 1) * super.blocksPerGroup - 1,
                group.freeBlocksCount,
                group.freeInodesCount,
                group.blockMap,
                (ulong) group.blockMap + LINN_GROUP_NUM_BLOCKMAP(&super),
                group.inodeMap,
                (ulong) group.inodeMap + LINN_GROUP_NUM_INODEMAP(&super),
                group.inodeTable,
                (ulong) group.inodeTable + LINN_GROUP_NUM_INODETAB(&super));
    }
    // Cleanup and terminate.
    fclose(fp);
    return EXIT_FAILURE;
}
