/*
 * Copyright (C) 2015 Niek Linnenbank
 * Copyright (C) 2013 Goswin von Brederlow <goswin-v-b@web.de>
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

#include <FreeNOS/System.h>

#define STACKSIZE   PAGESIZE

/* To keep this in the first portion of the binary. */
.section ".text.boot"

/* Make _start global. */
.globl _start, svcStack

/*
 * Entry point for the kernel.
 *
 * r15 -> should begin execution at 0x8000.
 * r0 -> 0x00000000
 * r1 -> 0x00000C42
 * r2 -> 0x00000100 - start of ATAGS
 * preserve these registers as argument for kernel_main
 */
_start:

    /*
     * Check if hypervisor mode is enabled. Some new ARM processors
     * offer hypervisor mode which is very limited in its instruction
     * and execution functionality. The processor must switch to system service mode (SVC)
     * before continuing further with bootstrap.
     */
    mrs r4, cpsr
    and r4, r4, #(MODE_MASK)
    teq r4, #(HYP_MODE)
    bne 1f
    mrs r4, cpsr
    bic r4, r4, #(MODE_MASK)
    orr r4, r4, #(SVC_MODE)
    orr r4, r4, #(IRQ_BIT | FIQ_BIT | ASYNC_ABORT_BIT)
    msr spsr_fsxc, r4
    adr lr, 1f
    ELR
    ERET
1:

#ifdef SMP
    /* Read CoreID */
    mrc p15, 0, r0, c0, c0, 5
    and r0, r0, #3
    cmp r0, #0
    beq 3f

2:  /* cpu 1..N */
    wfi
    b 2b
3:  /* cpu 0 */
#endif /* SMP */

    /* Initialize stack pointers for all ARM modes */
    msr cpsr_c, #(IRQ_MODE | IRQ_BIT | FIQ_BIT) /* IRQ mode */
    ldr sp, =irqStack

    msr cpsr_c, #(FIQ_MODE | IRQ_BIT | FIQ_BIT) /* FIQ mode */
    ldr sp, =fiqStack

    msr cpsr_c, #(SVC_MODE | IRQ_BIT | FIQ_BIT) /* SVC mode */
    ldr sp, =svcStack

    msr cpsr_c, #(ABT_MODE | IRQ_BIT | FIQ_BIT) /* Abort mode */
    ldr sp, =abtStack

    msr cpsr_c, #(UND_MODE | IRQ_BIT | FIQ_BIT) /* Undefined mode */
    ldr sp, =undStack

    msr cpsr_c, #(SYS_MODE | IRQ_BIT | FIQ_BIT) /* System mode */
    mov sp, #0x8000

    /* continue kernel in SVC mode, temporary stack */
    msr cpsr_c, #(SVC_MODE | IRQ_BIT | FIQ_BIT) /* SVC mode */
    ldr sp, =tmpStack

    /* Clear out bss. */
    ldr r4, =__bss_start
    ldr r9, =__bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    /* store multiple at r4. */
    stmia r4!, {r5-r8}

    /* If we are still below bss_end, loop. */
2:
    cmp r4, r9
    blo 1b

    /* Call kernel_main */
    ldr r3, =kernel_main
    blx r3

.section ".data"
.align 8
.fill STACKSIZE, 1, 0
irqStack:

.fill STACKSIZE, 1, 0
fiqStack:

.fill STACKSIZE, 1, 0
svcStack:

.fill STACKSIZE, 1, 0
abtStack:

.fill STACKSIZE, 1, 0
undStack:

.fill STACKSIZE, 1, 0
tmpStack:
