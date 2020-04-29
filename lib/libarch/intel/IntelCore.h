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

#ifndef __LIBARCH_INTEL_CPU_H
#define __LIBARCH_INTEL_CPU_H

#include <Types.h>
#include <Macros.h>
#include <Core.h>
#include "IntelIO.h"

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
 * Reads the CPU's timestamp counter.
 *
 * @return 64-bit integer.
 */
inline u64 timestamp()
{
    unsigned long long val;
    asm volatile ("rdtsc\n" : "=A"(val));
    return val;
}

/**
 * Reboot the system (by sending the a reset signal on the keyboard I/O port)
 */
#define cpu_reboot() \
({ \
    IntelIO io; \
    io.outb(0x64, 0xfe); \
})

/**
 * Shutdown the machine via ACPI.
 *
 * @note We do not have ACPI yet. Shutdown now has a bit naive implementation.
 * @see http://forum.osdev.org/viewtopic.php?t=16990
 */
#define cpu_shutdown() \
({ \
    IntelIO io; \
    io.outw(0xB004, 0x0 | 0x2000); \
})

/**
 * Puts the CPU in a lower power consuming state. 
 */
#define idle() \
    asm volatile ("hlt");

/**
 * Loads the Task State Register (LTR) with the given segment.
 *
 * @param sel TSS segment selector.
 */
#define ltr(sel) \
({ \
    u16 tr = sel; \
    asm volatile ("ltr %0\n" :: "r"(tr)); \
})

/**
 * Flushes the Translation Lookaside Buffers (TLB) for a single page.
 *
 * @param addr Memory address to flush.
 */
#define tlb_flush(addr) \
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory")

/**
 * Flushes all Translation Lookaside Buffers (TLB).
 */
#define tlb_flush_all() \
    asm volatile("mov %cr3, %eax\n" \
                 "mov %eax, %cr3\n")

/**
 * @group Intel CPU Exceptions
 * @{
 */

#define INTEL_DIVZERO   0
#define INTEL_DEBUGEX   1
#define INTEL_NMI       2
#define INTEL_BREAKP    3
#define INTEL_OVERFLOW  4
#define INTEL_BOUNDS    5
#define INTEL_OPCODE    6
#define INTEL_DEVERR    7
#define INTEL_DOUBLEF   8
#define INTEL_COSEG     9
#define INTEL_TSSERR    10
#define INTEL_SEGERR    11
#define INTEL_STACKERR  12
#define INTEL_GENERR    13
#define INTEL_PAGEFAULT 14
#define INTEL_FLOATERR  16
#define INTEL_ALIGNERR  17
#define INTEL_MACHCHK   18
#define INTEL_SIMD      19
#define INTEL_VIRTERR   20

/**
 * @}
 */

/**
 * @group Intel EFLAGS Register
 * @{
 */

#define INTEL_EFLAGS_DEFAULT    (1 << 1)
#define INTEL_EFLAGS_IRQ        (1 << 9)

/**
 * @}
 */

/**
 * Intel's Task State Segment.
 *
 * The TSS is mainly used for hardware context switching, which
 * the current implementation does not use. The only fields used
 * are the esp0 and ss0, which contain the kernel stack address and
 * segment used when processing an interrupt.
 */
typedef struct TSS
{
    u32 backlink;
    u32 esp0, ss0;
    u32 esp1, ss1;
    u32 esp2, ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax,ecx,edx,ebx;
    u32 esp, ebp;
    u32 esi, edi;
    u32 es, cs, ss, ds, fs, gs;
    u32 ldt;
    u32 bitmap;
}
TSS;

/**
 * Segment descriptor used in the GDT.
 */
typedef struct Segment
{
    u32 limitLow:16;
    u32 baseLow:16;
    u32 baseMid:8;
    u32 type:5;
    u32 privilege:2;
    u32 present:1;
    u32 limitHigh:4;
    u32 granularity:4;
    u32 baseHigh:8;
}
Segment;

/**
 * Segmentation registers.
 */
typedef struct SegRegs
{
    /* Segments. */
    u32 gs, fs, es, ds, ss0;
}
SegRegs;

/**
 * Structure represents the pusha/popa format.
 */
typedef struct CPURegs
{
    u32 edi, esi, ebp, esp0, ebx, edx, ecx, eax;
}
CPURegs;

/**
 * Privileged Interrupt Registers (ring 0)
 *
 * Consumed by a 'iretd'. Does not switch
 * privilege level, i.e. no ESP/SS for ring3 needed.
 */
typedef struct IRQRegs0
{
    u32 eip, cs, eflags;
}
IRQRegs0;

/**
 * Unprivileged Interrupt Registers (ring 3)
 *
 * Consumed by a 'iretd'. Changes the privilege
 * level to ring 3, by popping ESP/SS for ring3.
 *
 * Note that the stack pointer and stack segment
 * are needed in the privilege switch 'iretd', because
 * the stack contains the information needed for a
 * atomic privilege level switch.
 */
typedef struct IRQRegs3
{
    u32 eip, cs, eflags, esp3, ss3;
}
IRQRegs3;

/**
 * Contains all the CPU registers.
 */
typedef struct CPUState
{
    /* Segments. */
    SegRegs seg;

    /* By pusha */
    CPURegs regs;

    /* Vector/error arguments. */
    u32 vector, error;

    /* Pushed by processor. */
    IRQRegs3 irq;
}
CPUState;

/**
 * Intel CPU Core.
 */
class IntelCore : public Core
{
  public:

    /**
     * Log a CPU exception.
     *
     * @param state The current CPU state.
     */
    void logException(CPUState *state) const;

    /**
     * Log the CPU state.
     *
     * @param state The current CPU state.
     */
    void logState(CPUState *state) const;

    /**
     * Log a register.
     *
     * @param name Name of the register.
     * @param reg Value of the register.
     */
    void logRegister(const char *name, u32 reg) const;

    /**
     * Read the CR2 register.
     */
    volatile u32 readCR2() const;

    /**
     * Read the CR3 register.
     */
    volatile u32 readCR3() const;

    /**
     * Write the CR3 register
     */
    void writeCR3(u32 cr3) const;
};

#ifdef __KERNEL__

/** Global Descriptor Table. */
extern Segment gdt[];

/** Task State Segment. */
extern TSS kernelTss;

/** Kernel page directory. */
extern Address kernelPageDir[];

#endif /* __KERNEL__ */

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_CPU_H */
