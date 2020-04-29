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

#ifndef __LIB_LIBFS_PSEUDOFILE_H
#define __LIB_LIBFS_PSEUDOFILE_H

#include <Types.h>
#include "File.h"
#include "IOBuffer.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Pseudo files only exist in memory.
 */
class PseudoFile : public File
{
  public:

    /**
     * Default constructor.
     */
    PseudoFile();

    /**
     * Constructor.
     *
     * @param str Text string with file data
     */
    PseudoFile(const char *str);

    /**
     * Constructor with formatted input.
     *
     * @param format Format string.
     * @param ... Argument list.
     */
    PseudoFile(const char *format, ...);

    /**
     * Destructor.
     */
    virtual ~PseudoFile();

    /**
     * Read bytes from the file.
     *
     * @param buffer Output buffer.
     * @param size Number of bytes to read, at maximum.
     * @param offset Offset inside the file to start reading.
     *
     * @return Number of bytes read on success, Error on failure.
     *
     * @see IOBuffer
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Write bytes to the file.
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Number of bytes to write, at maximum.
     * @param offset Offset inside the file to start writing.
     *
     * @return Number of bytes written on success, Error on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Buffer from which we read. */
    char *m_buffer;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_PSEUDOFILE_H */
