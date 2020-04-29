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

#ifndef __ARM_SUNXI_H3_SYSTEM_H
#define __ARM_SUNXI_H3_SYSTEM_H

#define __SYSTEM
#define ARM
#define ARMV7
#define SUNXI
#define SUNXI_H3
#define SMP

#define IO_BASE 0x01000000
#define IO_SIZE (1024*1024*128)

#define UART_BASE       0x01c28000
#define GIC_DIST_BASE   0x01c81000
#define GIC_CPU_BASE    0x01c82000

#define UART0_IRQ       32
#define ARMTIMER_IRQ    29

#define RAM_ADDR (0x40000000)
#define RAM_SIZE (1024 * 1024 * 256)

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

#endif /* __ARM_SUNXI_H3_SYSTEM_H */
