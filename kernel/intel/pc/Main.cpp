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
#include <intel/IntelKernel.h>
#include <intel/IntelSerial.h>
#include <CoreInfo.h>
#include <Macros.h>
#include <Log.h>

extern C int kernel_main(CoreInfo *info)
{
    // Initialize heap at 3MB offset
    coreInfo.heapAddress = MegaByte(3);
    coreInfo.heapSize    = MegaByte(1);
    Kernel::heap(coreInfo.heapAddress, coreInfo.heapSize);

    // Start kernel debug serial console
    if (info->coreId == 0)
    {
        IntelSerial *serial = new IntelSerial(0x3f8);
        serial->setMinimumLogLevel(Log::Notice);
    }

    // Run all constructors first
    constructors();

    // Create and run the kernel
    IntelKernel *kernel = new IntelKernel(info);
    return kernel->run();
}
