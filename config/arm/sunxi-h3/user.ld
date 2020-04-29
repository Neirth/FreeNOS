/*
 * Copyright (C) 2019 Niek Linnenbank
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

ENTRY(_entry)
TARGET("elf32-littlearm")
OUTPUT_FORMAT("elf32-littlearm")

SECTIONS
{
    . = 0x80000000;

    .text :
    {
	*(.entry)
	*(.text)
	*(*.text)
	*(.gnu.linkonce.*)
	*(.data)
	*(.rodata)
	*(.rodata.*)
	*(.eh_frame)
        *(.note.gnu.build-id)

	. = ALIGN(4);
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
        KEEP (*(.dtors))
        KEEP (*(.fini_array.*))
        KEEP (*(.fini_array))
        LONG(0)
        DTOR_END = .;

        isKernel = .;
        LONG(0);
	. += 4;


	initStart = .;
	KEEP (*(SORT(.init*)))
	KEEP (*(.init*))
	initEnd   = .;

        . = ALIGN(4096);
        __bss_start = .;
        *(.bss)
        . = ALIGN(4096); /* align to page size */
        __bss_end = .;
    }
}
