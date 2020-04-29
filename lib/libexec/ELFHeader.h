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

#ifndef __LIBEXEC_ELFHEADER_H
#define __LIBEXEC_ELFHEADER_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libexec
 * @{
 */

/**
 * @name Identification indexes
 * @{
 */

/** Magic number byte 0 index. */
#define ELF_INDEX_MAGIC0        0

/** Magic number byte 1 index. */
#define ELF_INDEX_MAGIC1        1

/** Magic number byte 2 index. */
#define ELF_INDEX_MAGIC2        2

/** Magic number byte 3 index. */
#define ELF_INDEX_MAGIC3        3

/** File class index. */
#define ELF_INDEX_CLASS         4

/** Data encoding index. */
#define ELF_INDEX_DATA          5

/** File version index. */
#define ELF_INDEX_VERSION       6

/** Unused padding index. */
#define ELF_INDEX_PAD           7

/** Number of bytes in the ELF identity field. */
#define ELF_INDEX_NIDENT        16

/**
 * @}
 */

/**
 * @name Magic numbers
 * @{
 */

/** Magic number byte 0. */
#define ELF_MAGIC0      0x7f

/** Magic number byte 1. */
#define ELF_MAGIC1      'E'

/** Magic number byte 2. */
#define ELF_MAGIC2      'L'

/** Magic number byte 3. */
#define ELF_MAGIC3      'F'

/**
 * @}
 */

/**
 * @name Processor classes
 * @{
 */

/** Invalid class. */
#define ELF_CLASS_NONE  0

/** 32-bit objects. */
#define ELF_CLASS_32    1

/** 64-bit objects. */
#define ELF_CLASS_64    2

/**
 * @}
 */

/**
 * @name Data encoding (endianness)
 * @{
 */

/** Invalid data encoding. */
#define ELF_DATA_NONE   0

/** 2-complement, little endian. */
#define ELF_DATA_2LSB   1

/** 2-complement, big endian. */
#define ELF_DATA_2MSB   2

/**
 * @}
 */

/**
 * @name Object file types
 * @{
 */

/** No file type. */
#define ELF_TYPE_NONE   0

/** Relocatable file. */
#define ELF_TYPE_REL    1

/** Executable file. */
#define ELF_TYPE_EXEC   2

/** Shared object file. */
#define ELF_TYPE_DYN    3

/** Core file. */
#define ELF_TYPE_CORE   4

/** Number of defined types. */
#define ELF_TYPE_NUM    5

/** Processor-specific range start. */
#define ELF_TYPE_LOPROC 0xff00

/** Processor-specific range end */
#define ELF_TYPE_HIPROC 0xffff

/**
 * @}
 */

/**
 * @name Machine architectures
 * @{
 */

/** No machine. */
#define ELF_MACHINE_NONE        0

/** AT&T WE 32100. */
#define ELF_MACHINE_M32         1

/** SPARC. */
#define ELF_MACHINE_SPARC       2

/** Intel IBM-PC architecture. */
#define ELF_MACHINE_386         3

/** Motorola 68000. */
#define ELF_MACHINE_68K         4

/** Motorola 88000. */
#define ELF_MACHINE_88K         5

/** Intel 80860. */
#define ELF_MACHINE_860         7

/** MIPS RS3000 big endian. */
#define ELF_MACHINE_MIPS_RS3    8

/** MIPS RS4000 big endian. */
#define ELF_MACHINE_MIPS_RS4    10

/**
 * @}
 */

/**
 * @name Object file version.
 * @{
 */

/** Invalid version. */
#define ELF_VERSION_NONE        0

/** Current version. */
#define ELF_VERSION_CURRENT     1

/**
 * @}
 */

/**
 * Describes an ELF executable and must be placed at the beginning of executable programs.
 */
typedef struct ELFHeader
{
    /** Magic number and other info. */
    u8  ident[ELF_INDEX_NIDENT];

    /** Object file type. */
    u16 type;

    /** Physical machine architecture. */
    u16 machine;

    /** Object file version. */
    u32 version;

    /** Entry point virtual address. */
    u32 entry;

    /** Program header table file offset. */
    u32 programHeaderOffset;

    /* Section header table file offset */
    u32 sectionHeaderOffset;

    /* Processor-specific flags. */
    u32 flags;

    /** ELF header size in bytes. */
    u16 headerSize;

    /** Program header table entry size. */
    u16 programHeaderEntrySize;

    /** Program header table entry count. */
    u16 programHeaderEntryCount;

    /** Section header table entry size. */
    u16 sectionHeaderEntrySize;

    /** Section header table entry count. */
    u16 sectionHeaderEntryCount;

    /** Section header string table index. */
    u16 sectionHeaderStringsIndex;
}
ELFHeader;

/**
 * @name Segment types
 * @{
 */

/** Unused segment. */
#define ELF_SEGMENT_NULL        0

/** Loadable segment. */
#define ELF_SEGMENT_LOAD        1

/** Dynamic linker information. */
#define ELF_SEGMENT_DYNAMIC     2

/** Path to an interpreter for dynamic linking. */
#define ELF_SEGMENT_INTERP      3

/** Auxiliary information. */
#define ELF_SEGMENT_NOTE        4

/** Reserved. */
#define ELF_SEGMENT_SHLIB       5

/** Refers to the program segment header itself. */
#define ELF_SEGMENT_SELF        6

/** Reserved for processor-specific semantics. */
#define ELF_SEGMENT_LOPROC      0x70000000

/** Reserved for processor-specific semantics. */
#define ELF_SEGMENT_HIPROC      0x7fffffff

/**
 * @}
 */

/**
 * ELF program segment in the executable file.
 */
typedef struct ELFSegment
{
    /** Segment type. */
    u32 type;

    /** Offset in the file of this segment. */
    u32 offset;

    /** Virtual address start. */
    u32 virtualAddress;

    /** Physical address start. */
    u32 physicalAddress;

    /** Segment file image size. */
    u32 fileSize;

    /** Segment memory image size. */
    u32 memorySize;

    /** Optional segment flags. */
    u32 flags;

    /** Memory alignment when loaded into memory. */
    u32 alignment;
}
ELFSegment;

/**
 * @}
 * @}
 */

#endif /* __LIBEXEC_ELFHEADER_H */
