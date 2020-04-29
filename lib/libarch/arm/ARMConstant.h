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

#ifndef __LIBARCH_ARM_CONSTANT_H
#define __LIBARCH_ARM_CONSTANT_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * ARM CPU Constants
 * @{
 */

/** ARM is little endian. */
#define CPU_LITTLE_ENDIAN 1

/**
 * @}
 */

/**
 * ARM Program Status Register (CPSR)
 * @{
 */

#define USR_MODE    16
#define IRQ_MODE    18
#define FIQ_MODE    17
#define SVC_MODE    19
#define ABT_MODE    23
#define UND_MODE    27
#define SYS_MODE    31
#define HYP_MODE    0x1a
#define MODE_MASK   0x1f
#define IRQ_BIT         (1 << 7)
#define FIQ_BIT         (1 << 6)
#define ASYNC_ABORT_BIT (1 << 8)

/**
 * @}
 */

/**
 * ARM Model-Specific Instructions
 * @{
 */

/** msr ELR_hyp, lr */
#define ELR  .long 0xe12ef30e

/** eret */
#define ERET .long 0xe160006e

/**
 * @}
 */

/**
 * ARM Memory Constants.
 * @{
 */

/** Page entry bit shift. */
#define PAGESHIFT       12

/**
 * Page table bit shift.
 *
 * In ARM, the first-level page directory entry is selected
 * by bits [31:20] from the virtual address, plus two 0 bytes.
 *
 * @see ARM Architecture Reference Manual, page 724.
 */
#define DIRSHIFT        20

/** ARM uses 4K pages. */
#define PAGESIZE        4096

/**
 * Number of entries in the first-level page table.
 *
 * TTBR0 is the first-level page table for the application.
 * TTBR1 is always 16KB, the first-level page table of the kernel/OS.
 */
#define PAGEDIR_MAX     4096
#define PAGEDIR_SIZE    (PAGEDIR_MAX * sizeof(u32))

/**
 * Number of entries in a second-level page table.
 *
 * Second-level page tables are 1KB in size and map 1MB of virtual memory.
 * Second-level page tables must be aligned on 1KB boundaries.
 *
 * @see ARM Architecture Reference Manual, page 734.
 */
#define PAGETAB_MAX     256
#define PAGETAB_SIZE    (PAGETAB_MAX * sizeof(u32))
#define PAGETAB_SPAN    (PAGETAB_MAX*PAGESIZE)

/** Mask to find the page. */
#define PAGEMASK        0xfffff000 

/** Memory address alignment. */
#define MEMALIGN        4

/**
 * @}
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_CONSTANT_H */
