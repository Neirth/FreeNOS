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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <libgen.h>
#include <TerminalCodes.h>
#include <Runtime.h>
#include "ListFiles.h"

ListFiles::ListFiles(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("List files on the filesystem");
    parser().registerPositional("FILE", "Target file to list", 0);
    parser().registerFlag('l', "long", "List files in long output format");
    parser().registerFlag('a', "all", "List all files on the filesystem");
}

ListFiles::~ListFiles()
{
}

ListFiles::Result ListFiles::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();
    Result result = Success, ret = Success;

    // Refresh mountpoints on the filesystem first
    refreshMounts(0);

    // List files provided on the command-line, if any
    if (positionals.count() > 0)
    {
        for (Size i = 0; i < positionals.count(); i++)
        {
            result = printFiles(positionals[i]->getValue());

            // Update the return result
            if (result != Success)
            {
                ret = result;
            }
        }
    }
    // Otherwise, list the current directory
    else
    {
        char path[PATH_MAX];
        getcwd(path, sizeof(path));

        ret = printFiles(path);
    }

    // Done
    return ret;
}

ListFiles::Result ListFiles::printFiles(const String & path) const
{
    struct dirent *dent;
    struct stat st;
    char tmp[PATH_MAX];
    String out;
    DIR *d;
    Result r = Success;

    // Retrieve file status
    if (stat(*path, &st) != 0)
    {
        ERROR("failed to stat '" << *path << "': " << strerror(errno));
        return IOError;
    }

    // Is the given file a directory?
    if (S_ISDIR(st.st_mode))
    {
        // Attempt to open the directory
        if (!(d = opendir(*path)))
        {
            ERROR("failed to open '" << *path << "': " << strerror(errno));
            return IOError;
        }

        // Read directory
        while ((dent = readdir(d)))
        {
            // Construct full path
            snprintf(tmp, sizeof(tmp),
                    "%s/%s", *path, dent->d_name);

            if ((r = printSingleFile(tmp, out)) != Success)
                break;
        }
        // Close it
        closedir(d);
    }
    // The given file is not a directory
    else
    {
        r = printSingleFile(path, out);
    }

    // Final newline
    if (!arguments().get("long"))
        out << "\r\n";

    // Write to standard output
    write(1, *out, out.length());
    
    // Success
    return r;
}

ListFiles::Result ListFiles::printSingleFile(const String & path, String & out) const
{
    struct stat st;

    // Retrieve file status
    if (stat(*path, &st) != 0)
    {
        ERROR("failed to stat '" << *path << "': " << strerror(errno));
        return IOError;
    }

    // Apply long output
    if (arguments().get("long"))
    {
        out << WHITE;
        out << (st.st_mode & S_IRUSR ? "r" : "-");
        out << (st.st_mode & S_IWUSR ? "w" : "-");
        out << (st.st_mode & S_IXUSR ? "x" : "-");
        out << (st.st_mode & S_IRGRP ? "r" : "-");
        out << (st.st_mode & S_IWGRP ? "w" : "-");
        out << (st.st_mode & S_IXGRP ? "x" : "-");
        out << (st.st_mode & S_IROTH ? "r" : "-");
        out << (st.st_mode & S_IWOTH ? "w" : "-");
        out << (st.st_mode & S_IXOTH ? "x" : "-");

        out << " uid:" << st.st_uid << " ";
        out.pad(23);

        out << " gid:" << st.st_gid << " ";
        out.pad(33);

        out << " " << st.st_size << " ";
        out.pad(43);
    }

    // Apply coloring
    if (S_ISDIR(st.st_mode))
        out << BLUE;

    else if (S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode))
        out << YELLOW;

    // Is the file executable?
    else if (st.st_mode & 0100)
        out << GREEN;
    else
        out << WHITE;

    out << basename((char *) *path) << " ";

    // Long output needs a newline
    if (arguments().get("long"))
        out << "\r\n";

    // Done
    return Success;
}
