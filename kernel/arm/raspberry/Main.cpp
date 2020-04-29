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

#include <FreeNOS/Config.h>
#include <FreeNOS/Support.h>
#include <FreeNOS/System.h>
#include <Macros.h>
#include <arm/ARMControl.h>
#include <arm/broadcom/BroadcomInterrupt.h>
#include <arm/broadcom/BroadcomTimer.h>
#include <PL011.h>
#include <DeviceLog.h>
#include "RaspberryKernel.h"

extern Address __bootimg;

extern C int kernel_main(u32 r0, u32 r1, u32 r2)
{
    // Invalidate all caches now
    Arch::Cache cache;
    cache.invalidate(Cache::Unified);

#ifdef ARMV7
    // Raise the SMP bit for ARMv7
    ARMControl ctrl;
    ctrl.set(ARMControl::SMPBit);
#endif

    // Create local objects needed for the kernel
    Arch::MemoryMap mem;
    BootImage *bootimage = (BootImage *) &__bootimg;

    // Fill coreInfo
    MemoryBlock::set(&coreInfo, 0, sizeof(CoreInfo));
    coreInfo.bootImageAddress = (Address) (bootimage);
    coreInfo.bootImageSize    = bootimage->bootImageSize;
    coreInfo.kernel.phys      = RAM_ADDR;
    coreInfo.kernel.size      = MegaByte(4);
    coreInfo.memory.phys      = RAM_ADDR;
    coreInfo.memory.size      = RAM_SIZE;

    // Initialize heap at the end of the kernel (and after embedded boot image)
    coreInfo.heapAddress = coreInfo.bootImageAddress + coreInfo.bootImageSize;
    coreInfo.heapAddress &= PAGEMASK;
    coreInfo.heapAddress += PAGESIZE;
    coreInfo.heapSize = MegaByte(1);
    Kernel::heap(coreInfo.heapAddress, coreInfo.heapSize);

    // Run all constructors first
    constructors();

    // Open the serial console as default Log
    PL011 pl011(UART0_IRQ);
    pl011.initialize();

    DeviceLog console(pl011);
    console.setMinimumLogLevel(Log::Notice);

    // Create the kernel
    RaspberryKernel kernel(&coreInfo);

    // Run the kernel
    return kernel.run();
}
