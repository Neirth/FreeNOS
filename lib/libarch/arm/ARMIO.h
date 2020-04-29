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

#ifndef __LIBARCH_ARMIO_H
#define __LIBARCH_ARMIO_H

#include <Types.h>
#include <IO.h>
#include "ARMCore.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * Input/Output operations specific to the ARM architecture
 */
class ARMIO : public IO
{
  public:

    /**
     * Constructor
     */
    ARMIO();

    /**
     * Read a byte from a port.
     *
     * @param port The I/O port to read from.
     *
     * @return A byte read from the port.
     */
    inline u8 inb(u16 port)
    {
        return 0;
    }

    /**
     * Read a word from a port.
     *
     * @param port The I/O port to read from.
     *
     * @return Word read from the port.
     */
    inline u16 inw(u16 port)
    {
        return 0;
    }

    /**
     * Output a byte to a port.
     *
     * @param port Port to write to.
     * @param byte The byte to output.
     */
    inline void outb(u16 port, u8 byte)
    {
    }

    /**
     * Output a word to a port.
     *
     * @param port Port to write to.
     * @param byte The word to output.
     */
    inline void outw(u16 port, u16 word)
    {
    }

    /**
     * Output a long to a I/O port.
     *
     * @param port Target I/O port.
     * @param l The long 32-bit number to output.
     */
    inline void outl(u16 port, u32 l)
    {
    }

    /**
     * write to memory mapped I/O register
     */
    inline void write(u32 reg, u32 data)
    {
        dmb();
        u32 addr = reg + m_base;
        asm volatile("str %[data], [%[reg]]"
                 : : [reg]"r"(addr), [data]"r"(data));
        dmb();
    }

    /**
     * read from memory mapped I/O register
     *
     * @param reg Address to read
     *
     * @return 32-bit value
     */
    inline u32 read(u32 reg) const
    {
        dmb();
        u32 addr = reg + m_base;
        u32 data;
        asm volatile("ldr %[data], [%[reg]]"
                 : [data]"=r"(data) : [reg]"r"(addr));
        dmb();
        return data;
    }

    /**
     * Read a number of 32-bit values.
     *
     * @param addr Address of the starting 32-bit value.
     * @param count Number of bytes to read.
     * @param buf Output buffer.
     */
    inline void read(Address addr, Size count, void *buf) const
    {
        for (Size i = 0; i < count; i+= sizeof(u32))
        {
            *(u32 *)(((u8 *)buf) + i) = read(addr + i);
        }
    }

    /**
     * Write a number of 32-bit values.
     *
     * @param addr Address of the starting 32-bit value.
     * @param count Number of bytes to write.
     * @param buf Input buffer.
     */
    inline void write(Address addr, Size count, void *buf)
    {
        for (Size i = 0; i < count; i+= sizeof(u32))
        {
            write(addr + i, *(u32 *) (((u8 *)buf) + i));
        }
    }

    /**
     * Set bits in memory mapped register.
     *
     * @param addr Address of the register to write.
     * @param data 32-bit value containing the bits to set (bitwise or).
     */
    inline void set(Address addr, u32 data)
    {
        volatile u32 current = read(addr);
        current |= data;
        write(addr, current);
    }

    /**
     * Unset bits in memory mapped register.
     *
     * @param addr Address of the register to write.
     * @param data 32-bit value containing the bits to set (bitwise or).
     */
    inline void unset(Address addr, u32 data)
    {
        volatile u32 current = read(addr);
        current &= ~(data);
        write(addr, current);
    }
};

namespace Arch
{
    typedef ARMIO IO;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARMIO_H */
