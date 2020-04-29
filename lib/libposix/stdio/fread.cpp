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

#include <unistd.h>
#include <sys/types.h>
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"

size_t fread(void *ptr, size_t size,
             size_t nitems, FILE *stream)
{
    size_t num = 0, i;
    char *buf = (char *) ptr;

    // Read items
    for (i = 0; i < nitems; i++)
    {
        num += read(stream->fd, buf, size);
        buf += size;
    }

    // Done
    return num;
}
