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

#ifndef __LIBARCH_BOOTIMAGE_H
#define __LIBARCH_BOOTIMAGE_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

/** First magic byte. */
#define BOOTIMAGE_MAGIC0        ('F') + ('r' << 8) + ('e' << 16) + ('e' << 24)

/** Second magic byte. */
#define BOOTIMAGE_MAGIC1        ('N') + ('O' << 8) + ('S' << 16) + (0x1 << 24)

/** Version of the boot image layout. */
#define BOOTIMAGE_REVISION      2

/** Maximum length of BootSymbol names. */
#define BOOTIMAGE_NAMELEN       32

/**
 * BootImage contains executable programs to be loaded at system bootup.
 */
typedef struct BootImage
{
    /** Magic numbers to detect a valid boot image. */
    u32 magic[2];

    /** Version of the boot image layout. */
    u8  layoutRevision;

    /** Total size of the boot image in bytes */
    u32 bootImageSize;

    /** Offset of the symbol table. */
    u32 symbolTableOffset;

    /** Number of entries in the symbols table. */
    u16 symbolTableCount;

    /** Offset of the segments table. */
    u32 segmentsTableOffset;

    /** Number of entries in the segments table. */
    u16 segmentsTableCount;
}
BootImage;

/**
 * Boot symbol types.
 */
typedef enum BootSymbolType
{
    BootProgram     = 0, /**< Executable program */
    BootPrivProgram = 1, /**< Privileged executable program */
    BootFilesystem  = 2, /**< Embedded filesystem */
    BootData        = 3  /**< Binary data */
}
BootSymbolType;

/**
 * Program embedded in the BootImage.
 */
typedef struct BootSymbol
{
    /** Name of the boot symbol. */
    char name[BOOTIMAGE_NAMELEN];

    /** Type of boot symbol. */
    BootSymbolType type;

    /** Program entry point (only valid for BootProgram symbols). */
    u32 entry;

    /** Offset in the segments table. */
    u32 segmentsOffset;

    /** Number of contiguous entries in the segment table. */
    u16 segmentsCount;

    /** Total size of the BootSymbol segments */
    u32 segmentsTotalSize;
}
BootSymbol;

/**
 * Memory segment.
 */
typedef struct BootSegment
{
    /** Virtual memory address to load the segment. */
    u32 virtualAddress;

    /** Total size of the segment. */
    u32 size;

    /** Offset in the boot image of the segment contents. */
    u32 offset;
}
BootSegment;

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_BOOTIMAGE_H */
