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
#include <unistd.h>
#include <fcntl.h>
#include "Hostname.h"

Hostname::Hostname(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Print the system hostname");
}

Hostname::~Hostname()
{
}

Hostname::Result Hostname::exec()
{
    char host[128];

    // Fetch hostname
    gethostname(host, sizeof(host));
    
    // Output our hostname
    printf("%s\r\n", host);
    
    // Done
    return Success;
}
