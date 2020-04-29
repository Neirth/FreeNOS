/*
 * Copyright (C) 2009 Alexander Schrijver
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
#include "Echo.h"

Echo::Echo(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Echo standard input to output");
    parser().registerPositional("STRING", "text string to echo", 0);
    parser().registerFlag('n', "no-newline", "If set, do not print new line character");
}

Echo::~Echo()
{
}

Echo::Result Echo::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();
    const char *no_newline = arguments().get("no-newline");
    
    // Loop positional arguments
    for (Size i = 0; i < positionals.count(); i++)
        printf("%s ", *(positionals[i]->getValue()));

    // Print newline
    if (no_newline == NULL)
        printf("\n");

    return Success;
}
