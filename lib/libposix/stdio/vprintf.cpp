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
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

int vprintf(const char *format, va_list args)
{
    char buf[1024];
    Size size, written = 0;
    Error e;

    // Write formatted string
    size = vsnprintf(buf, sizeof(buf), format, args);

    // Write it to standard output
    while (written < size)
    {
        e = write(1, buf + written, size - written);

        switch (e)
        {
            // Error occurred
            case -1:
                return e;

            // End of file reached
            case 0:
                return written;

            // Process bytes
            default:
                written += e;
        }
    }

    // Done
    return written;
}
