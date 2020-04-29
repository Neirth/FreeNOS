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
#include <unistd.h>
#include "Sleep.h"

Sleep::Sleep(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Stop executing for some time");
    parser().registerPositional("SECONDS", "Stop executing for the given number of seconds");
}

Sleep::~Sleep()
{
}

Sleep::Result Sleep::exec()
{
    int sec = 0;

    // Convert input to seconds
    if ((sec = atoi(arguments().get("SECONDS"))) <= 0)
    {
        ERROR("invalid sleep time `" << arguments().get("SECONDS") << "'");
        return InvalidArgument;
    }

    // Sleep now
    if (sleep(sec) != 0)
    {
        ERROR("failed to sleep: " << strerror(errno));
        return IOError;
    }

    // Done
    return Success;
}
