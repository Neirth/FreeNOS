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

#include <string.h>
#include <limits.h>
#include "libgen.h"

char * dirname(char *path)
{
    static char buf[PATH_MAX];
    char *str;

    if (path[0])
    {
        if ((str = strrchr(path, '/')))
        {
            strlcpy(buf, path, (str-path) + 1);
        }
        else
            strlcpy(buf, path, PATH_MAX);
    }
    else
        strlcpy(buf, ".", PATH_MAX);

    return buf;
}
