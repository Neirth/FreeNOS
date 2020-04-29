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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <Runtime.h>
#include "Remove.h"

Remove::Remove(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Remove file from the filesystem");
    parser().registerPositional("FILE", "Name of the file(s) to remove", 0);
}

Remove::~Remove()
{
}

Remove::Result Remove::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();

    // Refresh the mounted filesystems
    refreshMounts(0);

    // Delete all given files
    for (Size i = 0; i < positionals.count(); i++)
    {
        const char *path = *(positionals[i]->getValue());

        // Attempt to remove the file
        if (unlink(path) < 0)
        {
            ERROR("failed to unlink '" << path << "': " << strerror(errno));
            return IOError;
        }
    }

    // Done
    return Success;
}
