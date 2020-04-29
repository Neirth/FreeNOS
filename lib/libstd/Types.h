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

#ifndef __TYPES_H
#define __TYPES_H

#ifndef __ASSEMBLER__

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Restrict bit operations.
 *
 * @see http://kerneltrap.org/node/3848
 */
#define BITWISE

/**
 * @name Unsigned integers.
 * @{
 */

/** Unsigned integer number. */
typedef unsigned int uint;

/** Unsigned long number. */
typedef unsigned long ulong;

/** Unsigned 64-bit number. */
typedef unsigned long long u64;

/** Unsigned 32-bit number. */
typedef unsigned int u32;

/** Unsigned 16-bit number. */
typedef unsigned short u16;

/** Unsigned 8-bit number. */
typedef unsigned char u8;

/**
 * @}
 */

/**
 * @name Signed integers.
 * @{
 */

/** Signed integer number. */
typedef signed int sint;

/** Signed long number. */
typedef signed long slong;

/** Signed 64-bit number. */
typedef signed long long s64;

/** Signed 32-bit number. */
typedef signed int s32;

/** Signed 16-bit number. */
typedef signed short int s16;

/** Signed 8-bit number. */
typedef signed char s8;

/**
 * @}
 */

/**
 * @name Bitwise checked integers.
 *
 * @see http://kerneltrap.org/node/3848
 * @{
 */

/** Unsigned 64-bit little endian number. */
typedef u64 BITWISE le64;

/** Unsigned 64-bit big endian number. */
typedef u64 BITWISE be64;

/** Unsigned 32-bit little endian number. */
typedef u32 BITWISE le32;

/** Unsigned 32-bit big endian number. */
typedef u32 BITWISE be32;

/** Unsigned 16-bit little endian number. */
typedef u16 BITWISE le16;

/** Unsigned 16-bit big endian number. */
typedef u16 BITWISE be16;

/** Unsigned 8-bit little endian number. */
typedef u8  BITWISE le8;

/** Unsigned 8-bit big endian number. */
typedef u8  BITWISE be8;

/**
 * @}
 */

/** Any sane size indicator cannot go negative. */
typedef unsigned int Size;

/** A memory address. */
typedef unsigned long Address;

/** User Identity. */
typedef unsigned short UserID;

/** Group Identity. */
typedef unsigned short GroupID;

/** Process Identification Number. */
typedef u16 ProcessID;

/**
 * Describes a device ID number.
 */
typedef struct DeviceID
{
    /** Major device ID number is a PID. */
    ProcessID major;

    /** Device specific minor ID number. */
    u16 minor;
}
DeviceID;

/**
 * Error code defined in Error.h
 * @see Error.h
 */
typedef slong Error;

#ifdef __cplusplus

namespace Number
{
    /**
     * Numeral system base type.
     */
    typedef enum Base
    {
        Dec, /** Decimal: 0-10 */
        Hex, /** Hexadecimal: 0x0-0xf */
    }
    Base;
};

#endif /* __cplusplus */

/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif /* __ASSEMBLER__ */
#endif /* __TYPES_H */
