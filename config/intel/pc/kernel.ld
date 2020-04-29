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

ENTRY(multibootEntry)
TARGET("elf32-i386")
OUTPUT_FORMAT("elf32-i386")

SECTIONS
{
    . = 0x00100000;
    kernelStart = .;

    .text :
    {
        *(.boot)
        *(.text)
        *(*.text)
        *(.gnu.linkonce.*)
        *(.rodata)
        *(.rodata.*)
        *(.eh_frame)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        *(.note.gnu.build-id)

        CTOR_LIST = .;
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*(.preinit_array))
        KEEP (*(.init_array.*))
        KEEP (*(.init_array))
        LONG(0)
        CTOR_END = .;

        DTOR_LIST = .;
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        KEEP (*(.fini_array.*))
        KEEP (*(.fini_array))
        LONG(0)
        DTOR_END = .;
        . += 4;

        initStart = .;
        KEEP (*(SORT(.init*)))
        initEnd   = .;
        isKernel = .;
        LONG(1);
        . = ALIGN(4096);
    }

    .bss :
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        __bss_end = .;
        *(COMMON)

        . += 4;
        kernelEnd = .;

        . = 0x00400000;
    }
}
