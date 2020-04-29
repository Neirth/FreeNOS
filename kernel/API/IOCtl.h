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

#ifndef __API_IOCTL_H
#define __API_IOCTL_H

#ifndef __SYSTEM
#error Do not include this file directly, use FreeNOS/System.h instead
#endif

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernelapi
 * @{
 */

/**
 * Available operation to perform using PrivExec().
 *
 * @see PrivExec
 */
typedef enum IOOperation
{
    IOByteRead  = 0,
    IOByteWrite = 1,
    IOWordRead  = 2,
    IOWordWrite = 3,
    IOLongWrite = 4
}
IOOperation;

/** Operator to print a IOOperation to a Log */
Log & operator << (Log &log, IOOperation op);

inline u8 ReadByte(Address addr)
{
    return (u8) trapKernel3(API::IOCtlNumber, IOByteRead, addr, 0);
}

inline API::Result WriteByte(Address addr, u8 value)
{
    return trapKernel3(API::IOCtlNumber, IOByteWrite, addr, value);
}

inline u16 ReadWord(Address addr)
{
    return (u16) trapKernel3(API::IOCtlNumber, IOWordRead, addr, 0);
}

inline API::Result WriteWord(Address addr, u16 value)
{
    return trapKernel3(API::IOCtlNumber, IOWordWrite, addr, value);
}

inline API::Result WriteLong(Address addr, ulong value)
{
    return trapKernel3(API::IOCtlNumber, IOLongWrite, addr, value);
}

/**
 * @}
 */

#ifdef __KERNEL__

/**
 * @addtogroup kernelapi_handler
 * @{
 */

/**
 * Kernel handler prototype for various I/O functions.
 *
 * @param op Operation to perform
 * @param addr Input/Output address
 * @param value Value for writing
 *
 * @return Error code
 */
extern Error IOCtlHandler(IOOperation op, Address addr, ulong value);

/**
 * @}
 */

#endif /* __KERNEL__ */

/**
 * @}
 */

#endif /* __API_PROCESSCTL_H */
