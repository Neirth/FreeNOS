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

#include "Directory.h"

Directory::Directory() : File(DirectoryFile)
{
    insert(DirectoryFile, ".");
    insert(DirectoryFile, "..");
}

Directory::~Directory()
{
    for (ListIterator<Dirent *> i(entries); i.hasCurrent(); i++)
        delete i.current();

    entries.clear();
}

Error Directory::read(IOBuffer & buffer, Size size, Size offset)
{
    Size bytes = 0;

    // Loop our list of Dirents
    for (ListIterator<Dirent *> i(&entries); i.hasCurrent(); i++)
    {
        // Can we read another entry?
        if (bytes + sizeof(Dirent) <= size)
        {
            buffer.write(i.current(), sizeof(Dirent), bytes);
            bytes += sizeof(Dirent);
        }
        else break;
    }
    // Report results
    return bytes;
}

File * Directory::lookup(const char *name)
{
    return ZERO;
}

void Directory::insert(FileType type, const char *name, ...)
{
    char path[PATHLEN];
    va_list args;
    Dirent *d;

    // Only insert if not already in
    if (!get(name))
    {
        // Format the path variable
        va_start(args, name);
        vsnprintf(path, sizeof(path), name, args);
        va_end(args);

        // Create an fill entry object
        d = new Dirent;
        strlcpy(d->name, path, DIRENT_LEN);
        d->type = type;
        entries.append(d);
        m_size += sizeof(*d);
    }
}

void Directory::remove(const char *name)
{
    for (ListIterator<Dirent *> i(&entries); i.hasCurrent(); i++)
    {
        if (strcmp(i.current()->name, name) == 0)
        {
            delete i.current();
            i.remove();
            m_size -= sizeof(Dirent);
            return;
        }
    }
}

void Directory::clear()
{
    for (ListIterator<Dirent *> i(entries); i.hasCurrent(); i++)
        delete i.current();

    entries.clear();
}

Dirent * Directory::get(const char *name)
{
    for (ListIterator<Dirent *> i(&entries); i.hasCurrent(); i++)
    {
        if (strcmp(i.current()->name, name) == 0)
        {
            return i.current();
        }
    }
    return (Dirent *) ZERO;
}
