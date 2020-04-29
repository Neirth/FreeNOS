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
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <Runtime.h>
#include "Cat.h"

Cat::Cat(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Concatenate files to standard output");
    parser().registerPositional("FILE", "file(s) to concatenate", 0);
}

Cat::~Cat()
{
}

Cat::Result Cat::initialize()
{
    return arguments().getPositionals().count() > 0 ? Success : ShowUsage;
}

Cat::Result Cat::exec()
{
    Result result = Success, ret = Success;
    const Vector<Argument *> & positionals = arguments().getPositionals();

    // Cat all given files. */
    for (Size i = 0; i < positionals.count(); i++)
    {
        // Perform cat. */
        result = cat(*(positionals[i]->getValue()));
    
        // Update exit code if needed
        if (result != Success)
        {
            ret = result;
        }
    }
    // Done
    return ret;
}

Cat::Result Cat::cat(const char *file) const
{
    char buf[PAGESIZE];
    int fd, e;
    struct stat st;
    const char *name = *(parser().name());

    DEBUG("file = " << file);

    // Retrieve fresh list of mountpoints
    refreshMounts(0);

    // Stat the file
    if (stat(file, &st) != 0)
    {
        printf("%s: failed to stat '%s': %s\r\n",
                name, file, strerror(errno));    
        return NotFound;
    }

    // Must be a regular file or device
    if (!S_ISREG(st.st_mode) && !S_ISCHR(st.st_mode))
    {
        printf("%s: not a file: '%s'\r\n", name, file);
        return InvalidArgument;
    }

    // Clear buffer
    memset(buf, 0, sizeof(buf));

    // Attempt to open the file first
    if ((fd = open(file, O_RDONLY)) < 0)
    {
        printf("%s: failed to open '%s': %s\r\n",
                name, file, strerror(errno));
        return IOError;
    }

    // Read contents
    while (1)
    {
        e = read(fd, buf, sizeof(buf) - 1);
        switch (e)
        {
            // Error occurred
            case -1:
                printf("%s: failed to read '%s': %s\r\n",
                        name, file, strerror(errno));
                close(fd);
                return IOError;
    
            // End of file
            case 0:
                close(fd);
                return Success;

            // Output data
            default:
                buf[e] = 0;
                printf("%s", buf);
                break;
        }
    }
    return InvalidArgument;
}
