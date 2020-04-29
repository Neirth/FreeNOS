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

#include "ARMConstant.h"

.globl vecTable, handlerTable, loadCoreState0

/*
 * ARM exception vector table.
 *
 * When an interrupt occurs, the ARM will jump to a predefined
 * memory location, which points effectively an entry in the table below.
 * The table below is normally loaded at the beginning of RAM. Each entry can only be 4 bytes
 * in size. The LDR (Load Register) instruction will load PC with the corresponding
 * address of the interrupt service routine from the interrupt routine table,
 * which is directly after the jump table in memory.
 *
 */
vecTable:

ldr pc, [pc, #0x18] /* Reset */
ldr pc, [pc, #0x18] /* Undefined Instruction */
ldr pc, [pc, #0x18] /* Software Interrupt */
ldr pc, [pc, #0x18] /* Prefetch Abort */
ldr pc, [pc, #0x18] /* Data Abort */
ldr pc, [pc, #0x18] /* Reserved */
ldr pc, [pc, #0x18] /* IRQ vector */
ldr pc, [pc, #0x18] /* FIQ vector */

.long(ex_reset)
.long(ex_undefined)
.long(ex_software)
.long(ex_prefetch)
.long(ex_data)
.long(ex_reserved)
.long(ex_irq)
.long(ex_fiq)

/**
 * General exception handler
 *
 * @see ARM1176JZF-S Technical Reference Manual, page 110, Table 2-8
 */
.macro callHandler index
    push {r14}            /* save calling PC (return address) */
    push {r0-r12}         /* save user registers */

    mov r2, sp            /* save SP and LR of user */
    stmdb r2, {sp, lr}^
    sub sp, sp, #8

    mrs r8, spsr
    push {r8}             /* save program status (SPSR) for user */

    mov r0, #\index       /* load function from handlerTable */
    ldr r1, =handlerTable
    mov r2, #4
    mla r1, r0, r2, r1
    ldr r0, [r1]
    mov lr, pc
    bx r0

loadCoreState\index:
    pop {r8}              /* restore program status (SPSR, will be moved in CPSR by movs instruction) */
    msr spsr, r8

    add sp, sp, #(8)      /* restore user SP and LR */
    mov r2, sp
    ldmdb r2, {sp, lr}^

    add sp, sp, #(13*4)   /* restore user registers */
    mov r2, sp
    ldmdb r2, {r0-r12}^

    pop {r14}             /* restore calling PC (return address) */

.endm

ex_reset:
callHandler 0
movs pc, lr

ex_undefined:
callHandler 1
movs pc, lr

ex_software:
callHandler 2
movs pc, lr

ex_prefetch:
callHandler 3
subs pc, r14, #4

ex_data:
callHandler 4
subs pc, r14, #8

ex_reserved:
callHandler 5
movs pc, lr

ex_irq:
sub lr, lr, #4
callHandler 6
movs pc, lr

ex_fiq:
callHandler 7
subs pc, r14, #4

/*
 * ARM exception handler table.
 *
 * Contains the addresses of C/C++ exception handler routines.
 */
handlerTable:
.fill 8, 4, 0
