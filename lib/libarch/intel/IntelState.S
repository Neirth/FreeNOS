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

#include "IntelConstant.h"

.global switchCoreState, loadCoreState, interruptRun, interruptHandler
.section ".text"

interruptRun:
    .long 0

switchCoreState:

    /* Setup correct stackframe. */
    push %ebp
    mov  %esp, %ebp

    /* Saves the current process state, if any. */
    mov 8(%ebp), %eax
    cmpl $0, %eax
    je restoreState

    /* Save core state on current stack. */
    pushf
    push %cs
    push $resume
    pusha

    /* Save stack pointer with saved state. */
    movl %esp, (%eax)

restoreState:

    /* Switch to new stack */
    movl 12(%ebp), %esp

    /* Restore kernel state. */
    popa
    iret

resume:
    pop %ebp
    ret

interruptHandler:

    /* Make a CPUState. */
    pusha
    pushl %ss
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    /* Switch to kernel data segment. */
    mov $KERNEL_DS_SEL, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    /* Process the interrupt. */
    movl $interruptRun, %eax
    call *(%eax)

loadCoreState:

    /* Restore data segments. */
    popl %gs
    popl %fs
    popl %es
    popl %ds
    add $4, %esp

    /* Continue program. */
    popa
    add $8, %esp
    iret
