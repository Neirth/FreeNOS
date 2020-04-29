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

#include <fcntl.h>
#include <sys/types.h>
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"

FILE * fopen(const char *filename,
             const char *mode)
{
    FILE *f;

    // Handle the file stream request
    switch (*mode)
    {
        // Read
        case 'r':
            f = (FILE *) malloc(sizeof(FILE));
            f->fd = open(filename, ZERO);
            return f;

        // Unsupported
        default:
            break;
    }

    // Sorry, not available yet!
    errno = ENOTSUP;
    return (FILE *) NULL;
}
