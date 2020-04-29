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

#include <String.h>
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

int vsnprintf(char *buffer, unsigned int size, const char *fmt, va_list args)
{
    char buf[20], *ptr;
    int ch, length = -1, i;
    unsigned int written = 0;
    String s;

    // Loop formatted message
    while ((ch = *fmt++) && written < size)
    {
        if (ch != '%')
        {
            *buffer++ = ch;
            written++;
        }
        else
        {
            switch_again:

            switch (*fmt)
            {
                // Length modifier
                case '0' ... '9':

                    for (i = 0; i < 19 && *fmt >= '0' && *fmt <= '9'; i++)
                    {
                        buf[i]   = *fmt++;
                        buf[i+1] = ZERO;
                    }
                    length = atoi(buf);
                    goto switch_again;

                // Integer
                case 'u':
                    s.setUnsigned(va_arg(args, unsigned), Number::Dec, buf);
                    ptr = buf;
                    goto string;

                case 'd':
                    itoa(buf, 10, va_arg(args, int));
                    ptr = buf;
                    goto string;

                // Long integer
                case 'l':
                    itoa(buf, 10, va_arg(args, long));
                    ptr = buf;
                    goto string;

                // Hexadecimal
                case 'x':
                    s.setUnsigned(va_arg(args, unsigned), Number::Hex, buf);
                    ptr = buf;
                    goto string;

                // Character
                case 'c':
                    buf[0] = va_arg(args, int);
                    buf[1] = ZERO;
                    ptr    = buf;
                    goto string;

                // String
                case 's':
                    ptr = va_arg(args, char *);

                    string:
                    while( ((length == -1 && *ptr) ||
                            (length > 0 && length--)) && written++ < size)
                    {
                        if (*ptr)
                        {
                            *buffer++ = *ptr++;
                        }
                        else
                            *buffer++ = ' ';
                    }
                    break;

                // Unsupported
                default:
                    *buffer++ = ch;
                    written++;
                    break;
            }
            fmt++;
            length = -1;
        }
    }

    // Null terminate
    if (written < size)
        *buffer = 0;
    return (written);
}
