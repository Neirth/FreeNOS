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

#ifndef __ARM_RASPBERRY_SYSTEM_H
#define __ARM_RASPBERRY_SYSTEM_H

/** Raspberry Pi 2 have a Broadcom 2836 SoC */
#define __SYSTEM
#define ARM
#define ARMV7
#define BCM2835
#define BCM2836
#define SMP

/** Default IO base on the Broadcom 2836 */
#define IO_BASE 0x3F000000
#define IO_SIZE (1024*1024*128)

#define GPIO_BASE (IO_BASE + 0x200000)
#define UART_BASE (GPIO_BASE + 0x1000)

/** Triggered when the PL011 asserts IRQ status */
#define UART0_IRQ 57

/** PhysicalTimer1, IRQ number. */
#define ARMTIMER_IRQ 3

#define RAM_ADDR (0x0)
#define RAM_SIZE (1024 * 1024 * 1024)

#include <arm/ARMConstant.h>

#ifndef __HOST__
#ifndef __ASSEMBLER__
#include <arm/ARMCore.h>
#include <arm/ARMCacheV7.h>
#include <arm/ARMIO.h>
#include <arm/ARMPaging.h>
#include <arm/ARMMap.h>
#include <arm/ARMKernel.h>
#include <arm/ARMProcess.h>

#include <FreeNOS/API.h>
#endif /* __ASSEMBLER__ */
#endif /* __HOST__ */

#endif /* __ARM_RASPBERRY_SYSTEM_H */
