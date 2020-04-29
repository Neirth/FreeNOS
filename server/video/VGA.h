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

#ifndef __VIDEO_VGA_SERVER_H
#define __VIDEO_VGA_SERVER_H

#include <DeviceServer.h>
#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup video
 * @{
 */

/** VGA physical video memory address. */
#define VGA_PADDR (0xb8000)

/** VGA I/O address port. */
#define VGA_IOADDR 0x3d4

/** VGA I/O data port. */
#define VGA_IODATA 0x3d5

/**
 * Encodes VGA attributes.
 *
 * @param front Front text color.
 * @param back Background color.
 */
#define VGA_ATTR(front,back) \
    (((back & 0xf) << 4) | ((front & 0xf)))

/**
 * Encodes a character for VGA output.
 *
 * @param ch Character to encode.
 * @param front Front text color.
 * @param back Background color.
 */
#define VGA_CHAR(ch,front,back) \
    ((VGA_ATTR(front,back) << 8) | (ch & 0xff))

/**
 * @brief VGA color attributes.
 */
enum Colors
{
    BLACK        = 0,
    BLUE         = 1,
    GREEN        = 2,
    CYAN         = 3,
    RED          = 4,
    MAGENTA      = 5,
    BROWN        = 6,
    LIGHTGREY    = 7,
    DARKGREY     = 8,
    LIGHTBLUE    = 9,
    LIGHTGREEN   = 10,
    LIGHTCYAN    = 11,
    LIGHTRED     = 12,
    LIGHTMAGENTA = 13,
    LIGHTBROWN   = 14,
    WHITE        = 15,
};

/**
 * @brief Video Graphics Array (VGA) support.
 *
 * This class implements a VGA driver. Applications may read and write
 * from the /dev/vga0 device file to retrieve and modify the current screen.
 * Currently the Terminal driver uses the /dev/vga device file to implement
 * the system console in FreeNOS.
 *
 * @see Terminal
 */
class VGA : public Device
{
    public:

    /**
     * @brief Class constructor function.
     *
     * @param width Number of characters horizontally.
     * @param height Number of characters vertically.
     */
    VGA(Size width = 80, Size height = 25);

    /**
     * @brief Initializes the VGA device.
     *
     * First this function maps the VGA video memory
     * into our address space. It clears the screen
     * and finally disables the VGA hardware cursor.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /**
     * @brief Read from VGA video memory.
     *
     * This function copies bytes directly from VGA
     * video memory into the target buffer.
     *
     * @param buffer Output buffer.
     * @param size Number of bytes to copy into the buffer.
     * @param offset Offset in VGA video memory to read in bytes.
     *
     * @return An error code describing the status of the operation.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * @brief Write to VGA video memory.
     *
     * This function copies bytes directly into VGA
     * video memory from the source buffer.
     *
     * @param buffer Input buffer.
     * @param size Number of bytes to copy from the buffer.
     * @param offset Offset in VGA video memory to write in bytes.
     *
     * @return An error code describing the status of the operation.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** @brief VGA video memory address. */
    u16 *vga;

    /** @brief Number of characters horizontally. */
    Size width;

    /** @brief Number of characters vertically. */
    Size height;
};

/**
 * @}
 * @}
 */

#endif /* __VIDEO_VGA_SERVER_H */
