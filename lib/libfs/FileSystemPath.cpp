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

#include <string.h>
#include "FileSystemPath.h"

/** Shortcut to the separator field. */
#define SEPARATOR(x) \
    (this->separator == (x))

/** Test for an End of Line. */
#define EOL(s,x) \
    ((x) == (s) + length - 1)

FileSystemPath::FileSystemPath()
    : m_fullPath(ZERO)
    , m_fullLength(0)
    , m_parentPath(ZERO)
    , m_separator(DEFAULT_SEPARATOR)
{
}

FileSystemPath::FileSystemPath(char *path, char separator)
    : m_fullPath(ZERO)
    , m_fullLength(ZERO)
    , m_parentPath(ZERO)
{
    parse(path, separator);
}

FileSystemPath::FileSystemPath(String *s, char separator)
    : m_fullPath(ZERO)
    , m_fullLength(ZERO)
    , m_parentPath(ZERO)
{
    parse((char *) **s, separator);
}

FileSystemPath::~FileSystemPath()
{
    for (ListIterator<String *> i(m_path); i.hasCurrent(); i++)
        delete i.current();

    m_path.clear();

    if (m_parentPath) delete m_parentPath;
    if (m_fullPath) delete m_fullPath;
}

void FileSystemPath::parse(const char *p, char sep)
{
    const char *cur   = p;
    char *parentStr   = ZERO;

    // Skip heading separators
    while (*cur && *cur == sep) cur++;

    // Save parameters
    p            = (char *) cur;
    m_separator  = sep;
    m_fullLength = strlen((char *)cur);
    m_fullPath   = new String(cur);
    String str(p);

    // Split the path into parts
    List<String> parts = str.split(sep);
    for (ListIterator<String> i(parts); i.hasCurrent(); i++)
        m_path.append(new String(i.current()));

    // Create parent, if any
    if (m_path.head() && m_path.head()->next)
    {
        // Allocate buffer
        parentStr  = new char[strlen(p)];
        memset(parentStr, 0, strlen(p));

        // Construct parent path
        for (List<String *>::Node *l = m_path.head(); l && l->next; l = l->next)
        {
            strcat(parentStr, **l->data);
            if (l->next && l->next->next)
                strncat(parentStr, &m_separator, 1);
        }
        // Save the path, then release buffer
        m_parentPath = new String(parentStr);
        delete parentStr;
    }
}

String * FileSystemPath::parent() const
{
    return m_parentPath;
}

String * FileSystemPath::base() const
{
    return m_path.last();
}

String * FileSystemPath::full() const
{
    return m_fullPath;
}

List<String *> * FileSystemPath::split()
{
    return &m_path;
}

Size FileSystemPath::length() const
{
    return m_fullLength;
}
