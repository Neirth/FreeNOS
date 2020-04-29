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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <String.h>
#include "ExternalTest.h"
#include "TestSuite.h"
#include "DirectoryScanner.h"

DirectoryScanner::DirectoryScanner(int argc, char **argv)
{
    m_argc = argc;
    m_argv = argv;
}

int DirectoryScanner::scan(const char *path)
{
    DIR *d;
    struct dirent *dent;
    char subPath[255];

    // Attempt to open the target directory.
    if (!(d = opendir(path)))
    {
        printf("%s: failed to open '%s': %s\r\n",
            m_argv[0], path, strerror(errno));
        return EXIT_FAILURE;
    }
    // Read directory.
    while ((dent = readdir(d)))
    {
        snprintf(subPath, sizeof(subPath), "%s/%s", path, dent->d_name);
        String str = subPath;

        // Check filetype
        switch (dent->d_type)
        {
            // Directory
            case DT_DIR:
                if (dent->d_name[0] != '.')
                    scan(subPath);
                break;

            // Regular file
            case DT_REG:
                if (str.endsWith((const char *)"Test"))
                    new ExternalTest(subPath, m_argc, m_argv);
                break;

            default:
                break;
        }
    }
    // Close it.
    closedir(d);
    return EXIT_SUCCESS;
}
