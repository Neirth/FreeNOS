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
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>
#include "UnixName.h"

UnixName::UnixName(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Print the operating system name");
    parser().registerFlag('s', "system", "Print the system name");
    parser().registerFlag('n', "node", "Print the node name");
    parser().registerFlag('r', "release", "Print the operating system release");
    parser().registerFlag('m', "machine", "Print the machine name");
    parser().registerFlag('a', "all", "Print all available information");
}

UnixName::~UnixName()
{
}

UnixName::Result UnixName::exec()
{
    struct utsname info;

    // Retrieve version information
    if (uname(&info) < 0)
    {
        ERROR("uname() failed: " << strerror(errno));
        return IOError;
    }

    // If no arguments given, just show the system name. */
    if (arguments().getFlags().count() == 0)
    {
        printf("%s", info.sysname);
    }
    // Print everything?
    else if (arguments().get("all"))
    {
        printf("%s %s %s %s %s ",
                info.sysname,
                info.nodename,
                info.release,
                info.version,
                info.machine);
    }
    // Otherwise, interpret argument(s).
    else
    {
        // System name
        if (arguments().get("system"))
            printf("%s ", info.sysname);

        // Node name
        if (arguments().get("node"))
            printf("%s ", info.nodename);

        // Release
        if (arguments().get("release"))
            printf("%s ", info.release);

        // Machine name
        if (arguments().get("machine"))
            printf("%s ", info.machine);
    }

    // Terminate with a newline
    printf("\r\n");
    
    // Done
    return Success;
}
