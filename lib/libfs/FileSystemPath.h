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

#ifndef __FILESYSTEM_FILESYSTEMPATH_H
#define __FILESYSTEM_FILESYSTEMPATH_H

#include <List.h>
#include <ListIterator.h>
#include <Types.h>
#include <String.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/** Maximum length of a filesystem path. */
#define PATHLEN 64

/** The default FileSystemPath separator. */
#define DEFAULT_SEPARATOR '/'

/**
 * Simple filesystem path parser.
 */
class FileSystemPath
{
  public:

    /**
     * Default constructor.
     */
    FileSystemPath();

    /**
     * Alternative Constructor.
     *
     * @param path The input path to parse.
     * @param separator Pathname separator.
     */
    FileSystemPath(char *path, char separator = DEFAULT_SEPARATOR);

    /**
     * Constructor using a String.
     *
     * @param s String containing the path to parse.
     * @param separator Pathname separator.
     */
    FileSystemPath(String *s, char separator = DEFAULT_SEPARATOR);

    /**
     * Destructor.
     */
    virtual ~FileSystemPath();

    /**
     * Parses a given character string as the path.
     *
     * @param p Path to parse.
     * @param separator Pathname separator.
     */
    void parse(const char *p, char sep = DEFAULT_SEPARATOR);

    /**
     * Retrieve the full path of our parent.
     *
     * @return Path of our parent.
     */
    String * parent() const;

    /**
     * The name of the last element in the path.
     *
     * @return Name of the base.
     */
    String * base() const;

    /**
     * Get the full path as a String instance.
     *
     * @return String instance.
     */
    String * full() const;

    /**
     * Returns a List of separate path elements.
     *
     * @return Pointer to a List.
     */
    List<String *> * split();

    /**
     * Get Length of our full path.
     *
     * @return Length.
     */
    Size length() const;

  private:

    /** The path split in pieces. */
    List<String *> m_path;

    /** Full input path. */
    String *m_fullPath;

    /** Full length of the given path. */
    Size m_fullLength;

    /** Full path to our parent. */
    String * m_parentPath;

    /** Separator character. */
    char m_separator;
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILESYSTEMPATH_H */
