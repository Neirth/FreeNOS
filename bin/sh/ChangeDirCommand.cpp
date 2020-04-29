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

#include "ChangeDirCommand.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <Log.h>
#include <Runtime.h>

ChangeDirCommand::ChangeDirCommand()
    : ShellCommand("cd", 1)
{
    m_help = "Change the current working directory";
}

int ChangeDirCommand::execute(Size nparams, char **params)
{
    refreshMounts(0);

    if (chdir(params[0]) != 0)
    {
        ERROR(getName() << ": failed to change directory to `" << params[0] << "': " << strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
