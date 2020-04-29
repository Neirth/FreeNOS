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

#ifndef __FILESYSTEM_FILE_SYSTEM_REQUEST_H
#define __FILESYSTEM_FILE_SYSTEM_REQUEST_H

#include "FileSystemMessage.h"
#include "IOBuffer.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Encapsulates a pending FileSystemMessage.
 */
class FileSystemRequest
{
  public:

    /**
     * Constructor
     */
    FileSystemRequest(FileSystemMessage *msg);

    /**
     * Destructor
     */
    virtual ~FileSystemRequest();

    /**
     * Get message.
     *
     * @return FileSystemMessage pointer
     */
    FileSystemMessage * getMessage();

    /**
     * Get IOBuffer.
     *
     * @return IOBuffer reference
     */
    IOBuffer & getBuffer();

  private:

    /** Message that was received */
    FileSystemMessage m_msg;

    /** Wrapper for doing I/O on the FileSystemMessage buffer. */
    IOBuffer *m_ioBuffer;
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILE_SYSTEM_REQUEST_H */
