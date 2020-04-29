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

#include <unistd.h>
#include <fcntl.h>
#include "StdioCommand.h"

StdioCommand::StdioCommand() : ShellCommand("stdio", 2)
{
    m_help = "Change standard I/O of the shell";
}

int StdioCommand::execute(Size nparams, char **params)
{
    // Close current standard I/O
    close(0);
    close(1);
    close(2);

    // Reopen standard I/O
    open(params[0], O_RDWR);
    open(params[1], O_RDWR);
    open(params[1], O_RDWR);

    // Done
    return 0;
}
