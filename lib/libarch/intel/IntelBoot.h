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

#ifndef __LIBARCH_INTEL_BOOT_H
#define __LIBARCH_INTEL_BOOT_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/** The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/** The flags for the Multiboot header. */
#define MULTIBOOT_HEADER_FLAGS          0x00000003

/** Size of the multiboot header structure. */
#define MULTIBOOT_HEADER_SIZE           52

/** The magic number passed by a Multiboot-compliant boot loader.  */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/**
 * @}
 * @}
 * @}
 */

/* Do not include in assembler source. */
#ifndef __ASSEMBLER__

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * The symbol table for a.out.
 */
typedef struct AoutSymbolTable
{
    u32 tabSize;
    u32 strSize;
    u32 address;
    u32 reserved;
}
AoutSymbolTable;

/**
 * The section header table for ELF.
 */
typedef struct ElfSectionHeaderTable
{
    u32 num;
    u32 size;
    u32 address;
    u32 shndx;
}
ElfSectionHeaderTable;

/**
 * The Multiboot information.
 */
typedef struct MultibootInfo
{
    u32 flags;
    u32 memLower;
    u32 memUpper;
    u32 bootDevice;
    u32 cmdline;
    u32 modsCount;
    u32 modsAddress;

    union
    {
        AoutSymbolTable aout;
        ElfSectionHeaderTable elf;
    };
    u32 mmapLength;
    u32 mmapAddress;
}
MultibootInfo;

/**
 * The module class.
 */
typedef struct MultibootModule
{
    u32 modStart;
    u32 modEnd;
    u32 string;
    u32 reserved;
}
MultibootModule;

/**
 * The MultiBoot memory map.
 *
 * @note Be careful that the offset 0 is base_addr_low but no size.
 */
typedef struct MultibootMemoryMap
{
    u32 size;
    u32 baseAddressLow;
    u32 baseAddressHigh;
    u32 lengthLow;
    u32 lengthHigh;
    u32 type;
}
MultibootMemoryMap;

/** Fill in by the early boot process. */
extern MultibootInfo multibootInfo;

/**
 * Entry point in 16-bit real mode.
 */
extern C void bootEntry16();

/**
 * Entry point in 32-bit protected mode.
 */
extern C void bootEntry32();

/**
 * Entry point from GRUB multiboot.
 *
 * The Bootstrap Processor (BSP) starts from GRUB.
 * This routine transforms the GRUB multiboot info to a 
 * CoreInfo and then continues with bootEntry32.
 *
 * @see bootEntry32
 */
extern C void multibootEntry();

/**
 * Convert multiboot info to a CoreInfo struct.
 */
extern C void multibootToCoreInfo(MultibootInfo *info);

/**
 * @}
 * @}
 * @}
 */

#endif /* !__ASSEMBLER__ */
#endif /* __LIBARCH_INTEL_BOOT_H */
