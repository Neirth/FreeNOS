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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <Runtime.h>
#include <Log.h>
#include "FileStatus.h"

FileStatus::FileStatus(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Retrieve file status from the filesystem");
    parser().registerPositional("FILE", "Name of the file(s) to stat", 0);
}

FileStatus::~FileStatus()
{
}

FileStatus::Result FileStatus::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();
    Result result = Success;
    Result ret = Success;

    // Refresh mounted file systems
    refreshMounts(0);

    // Perform a stat for each file
    for (Size i = 0; i < positionals.count(); i++)
    {
        // Stat the file immediately
        result = printStatus(positionals[i]->getValue());

        // Update exit status, if needed
        if (result != Success)
        {
            ret = result;
        }
    }    

    // Done
    return ret;
}

FileStatus::Result FileStatus::printStatus(const String & file) const
{
    struct stat st;

    // Try to stat the file
    if ((stat(*file, &st)) < 0)
    {
        ERROR("failed to stat `" << *file << "': " << strerror(errno));
        return IOError;
    }

    // Output file statistics
    printf("File: %s\r\n", *file);
    printf("Type: ");
    
    // Print the right file type
    if (S_ISREG(st.st_mode))
        printf("Regular File\r\n");
    
    else if (S_ISDIR(st.st_mode))
        printf("Directory\r\n");
    
    else if (S_ISCHR(st.st_mode))
    {
        printf("Character Device\r\n");
        printf("Major ID: %u\r\n", st.st_dev.major);
        printf("Minor ID: %u\r\n", st.st_dev.minor);
    }
    else if (S_ISBLK(st.st_mode))
    {
        printf("Block Device\r\n");
        printf("Major ID: %u\r\n", st.st_dev.major);
        printf("Minor ID: %u\r\n", st.st_dev.minor);
    }
    else
        printf("Unknown\r\n");

    // Print additional file information fields
    printf("Mode: %u\r\n", st.st_mode);
    printf("Size: %u\r\n", st.st_size);   
    printf("Uid:  %u\r\n", st.st_uid);
    printf("Gid:  %u\r\n", st.st_gid);

    // Done
    return Success;
}
