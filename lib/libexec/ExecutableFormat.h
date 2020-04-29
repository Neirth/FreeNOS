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

#ifndef __LIBEXEC_EXECUTABLEFORMAT_H
#define __LIBEXEC_EXECUTABLEFORMAT_H
#ifndef __ASSEMBLER__

#include <Memory.h>
#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libexec
 * @{
 */

/** Entry point of a program. */
typedef Address EntryPoint;

/** Forward declaration. */
class ExecutableFormat;

/**
 * Confirms if we understand the given format.
 * @return true on success and false on failure.
 */
typedef ExecutableFormat * FormatDetector(u8 *image, Size size);

/**
 * Abstraction class of various executable formats.
 */
class ExecutableFormat
{
  public:

    /**
     * Memory region.
     */
    typedef struct Region
    {
        Address virt;
        Size size;
        Memory::Access access;
        u8 *data;
    }
    Region;

    /**
     * Result code
     */
    enum Result
    {
        Success,
        NotFound,
        InvalidFormat,
        OutOfMemory
    };

  public:

    /**
     * Class constructor.
     *
     * @param image Pointer to program image.
     * @param size Size of the program image.
     */
    ExecutableFormat(const u8 *image, Size size);

    /**
     * Class destructor.
     */
    virtual ~ExecutableFormat();

    /**
     * Memory regions a program needs at runtime.
     *
     * @param regions Memory regions to fill.
     * @param count On input, the maximum number of regions to read.
     *              On output, the actual number of regions read.
     *
     * @return Result code.
     */
    virtual Result regions(Region *regions, Size *count) const = 0;

    /**
     * Lookup the program entry point.
     *
     * @param entry Entry point on output.
     * @return Result code.
     */
    virtual Result entry(Address *entry) const = 0;

    /**
     * Find a ExecutableFormat which can handle the given format.
     *
     * @param image Program image to read.
     * @param size Program image size.
     * @param fmt ExecutableFormat object pointer on output.
     * @return Result code.
     */
    static Result find(const u8 *image, Size size, ExecutableFormat **fmt);

  protected:

    /** Input image raw data */
    const u8 *m_image;

    /** Input image size in bytes */
    const Size m_size;
};

/**
 * @}
 * @}
 */

#endif /* __ASSEMBLER__ */
#endif /* __LIBEXEC_EXECUTABLEFORMAT_H */
