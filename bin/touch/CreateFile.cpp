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
#include "CreateFile.h"

CreateFile::CreateFile(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Create new files the filesystem");
    parser().registerPositional("FILE", "Name of the file(s) to create", 0);
}

CreateFile::~CreateFile()
{
}

CreateFile::Result CreateFile::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();
    Result result = Success;
    Result ret = Success;

    // Refresh mounted file systems
    refreshMounts(0);

    // Perform a stat for each file
    for (Size i = 0; i < positionals.count(); i++)
    {
        // Create the file immediately
        result = createFile(positionals[i]->getValue());

        // Update exit status, if needed
        if (result != Success)
        {
            ret = result;
        }
    }    

    // Done
    return ret;
}

CreateFile::Result CreateFile::createFile(const String & file) const
{
    // Attempt to create the file. */
    if (creat(*file, S_IRUSR|S_IWUSR) < 0)
    {
        ERROR("failed to create file `" << *file << "': " << strerror(errno));
        return IOError;
    }

    // Done
    return Success;
}
