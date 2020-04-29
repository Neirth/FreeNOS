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

#ifndef __LIB_LIBFS_IOBUFFER_H
#define __LIB_LIBFS_IOBUFFER_H

#include <FreeNOS/System.h>
#include <Types.h>
#include "FileSystemMessage.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * @brief Abstract Input/Output buffer.
 */
class IOBuffer
{
  public:

    /**
     * Constructor.
     *
     * @param msg Describes the request being processed.
     */
    IOBuffer(const FileSystemMessage *msg);

    /**
     * Destructor.
     */
    virtual ~IOBuffer();

    /**
     * Get byte count.
     *
     * @return Byte count of the internal buffer.
     */
    Size getCount() const;

    /**
     * Get filesystem message.
     *
     * @return FileSystemMessage pointer.
     */
    const FileSystemMessage * getMessage() const;

    /**
     * Get raw buffer.
     *
     * @return Buffer pointer.
     */
    const u8 * getBuffer() const;

    /**
     * @brief Read bytes from the I/O buffer.
     *
     * @param buffer Copy bytes from the I/O buffer to this memory address.
     * @param size Number of bytes to copy.
     * @param offset The offset inside the I/O buffer to start reading.
     *
     * @return Number of bytes read on success, and error code on failure.
     */
    Error read(void *buffer, Size size, Size offset = ZERO) const;

    /**
     * Write bytes to the I/O buffer.
     *
     * @param buffer Contains the bytes to write.
     * @param size Number of bytes to write.
     * @param offset The offset inside the I/O buffer to start writing.
     *
     * @return Number of bytes written on success, and error code on failure.
     */
    Error write(void *buffer, Size size, Size offset = ZERO) const;

    /**
     * Buffered read bytes from message to the I/O buffer.
     *
     * @return Error code.
     */
    Error bufferedRead();

    /**
     * Buffered write bytes to the I/O buffer.
     *
     * @param buffer Contains the bytes to write.
     * @param size Number of bytes to write.
     * @param offset The offset inside the I/O buffer to start writing.
     *
     * @return Number of bytes written on success, and error code on failure.
     */
    Error bufferedWrite(const void *buffer, Size size);

    /**
     * Flush write buffers.
     *
     * @return Error code.
     */
    Error flush() const;

    /**
     * Byte index operator.
     *
     * @param index Index value
     *
     * @return Byte value at the given index or 0 if index is invalid.
     */
    u8 operator[] (Size index) const;

  private:

    /**
     * @brief Current request being processed.
     *
     * Read() and write() will use fields from the current request
     * to fill in arguments for VMCopy().
     *
     * @see VMCopy
     * @see IOBuffer::read
     * @see IOBuffer::write
     */
    const FileSystemMessage *m_message;

    /** Buffer for storing temporary data. */
    u8 *m_buffer;

    /** Buffer size. */
    Size m_size;

    /** Bytes written to the buffer. */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_IOBUFFER_H */
