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

#include "IntelConstant.h"
#include "IntelBoot.h" 

/** Initial stack size */
#define STACK_SIZE PAGESIZE

.global multibootEntry, multibootHeader

.section ".boot"

/*
 * Multiboot header, 32-bit aligned.
 */
.align 4

multibootHeader:
        .long  (MULTIBOOT_HEADER_MAGIC)
        .long  (MULTIBOOT_HEADER_FLAGS)
        .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
        .long  (multibootHeader)

.section ".text"
.code32

/**
 * Entry point.
 */
multibootEntry:

    /* Disable interrupts. */
    cli

    /* Check multiboot magic */
    cmpl $MULTIBOOT_BOOTLOADER_MAGIC, %eax
    jnz useCoreInfo

    /* Setup temporary boot stack. */
    movl $(stack + STACK_SIZE), %esp
    movl %esp, %ebp

    /* Save multiboot info on the stack (for multibootToCoreInfo) */
    push %ebx

    /* Clear the BSS and then copy multiboot info to CoreInfo.
     * Note that for the secondary processors, the BSS is automatically
     * cleared to zero when copying the kernel itself with the CoreServer. */
    call clearBSS
    call multibootToCoreInfo

    /* Jump to bootEntry32 */
    movl $coreInfo, %eax
    jmp bootEntry32

useCoreInfo:

    /* We started using a CoreInfo (AP core) */
    movl %eax, %ebx
    addl $12, %ebx
    movl (%ebx), %ebx

    /* Jump to bootEntry32 for this core */
    movl $bootEntry32, %ecx
    addl %ebx, %ecx
    jmp *%ecx

/**
 * Stop execution immediately.
 */
halt:
    cli
    hlt
    jmp halt

.section ".data"
.align PAGESIZE

stack: .fill STACK_SIZE, 1, 0
