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

#include "stdlib.h"
#include "string.h"
#include "sys/types.h"

char * strndup(const char *s, size_t size)
{
    size_t bytes;
    char *buff;

    // Fetch input string length
    bytes = strlen(s);

    // Calculate bytes to allocate
    if (bytes > size)
    {
        bytes = size;
    }

    // Allocate buffer
    if ((buff = (char *) malloc(bytes + 1)) == NULL)
    {
        return (char *) NULL;
    }

    // Copy input into new buffer
    memcpy(buff, s, bytes + 1);

    // Null terminate
    buff[bytes] = 0;

    // Done
    return buff;
}
