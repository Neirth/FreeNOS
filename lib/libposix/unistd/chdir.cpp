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
#include <List.h>
#include <FileSystemPath.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Runtime.h"

int chdir(const char *filepath)
{
    String *last = ZERO;
    List<String *> lst;
    char cwd[PATH_MAX], buf[PATH_MAX], *path = ZERO;
    FileSystemPath fspath;
    struct stat st;

    // What's the current working dir?
    getcwd(cwd, PATH_MAX);

    // Relative or absolute?
    if (filepath[0] != '/')
    {
        snprintf(buf, sizeof(buf), "%s/%s", cwd, filepath);
        fspath.parse(buf);
        memset(buf, 0, sizeof(buf));

        // Process '..'
        for (ListIterator<String *> i(fspath.split()); i.hasCurrent(); i++)
        {
            if ((**i.current())[0] != '.')
            {
                lst.append(i.current());
                last = i.current();
            }
            else if ((**i.current())[1] == '.' && last)
            {
                lst.remove(last);
            }
        }

        // Construct final path
        for (ListIterator<String *> i(&lst); i.hasCurrent(); i++)
        {
            strcat(buf, "/");
            strcat(buf, **i.current());
        }
        path = buf;
    }
    else
        path = (char *) filepath;

    // Fall back to slash?
    if (!path[0])
    {
        strcpy(buf, "/");
        path = buf;
    }

    // Stat the file
    if (stat(path, &st) != 0)
    {
        return -1;
    }

    // Must be a directory
    if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        return -1;
    }

    // Set current directory
    (*getCurrentDirectory()) = path;

    // Done
    errno = ZERO;
    return 0;
}
