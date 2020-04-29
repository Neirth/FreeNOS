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

#include <FreeNOS/System.h>
#include <FreeNOS/Config.h>
#include <FreeNOS/Kernel.h>
#include <SplitAllocator.h>
#include <CoreInfo.h>

API::Result SystemInfoHandler(SystemInformation *info)
{
    SplitAllocator *memory = Kernel::instance->getAllocator();
    CoreInfo *core = Kernel::instance->getCoreInfo();

    DEBUG("");

    // Fill in our current information
    info->version          = VERSIONCODE;
    info->memorySize       = memory->size();
    info->memoryAvail      = memory->available();
    info->coreId           = core->coreId;

    info->bootImageAddress = core->bootImageAddress;
    info->bootImageSize    = core->bootImageSize;
    info->timerCounter     = core->timerCounter;
    info->coreChannelAddress = core->coreChannelAddress;
    info->coreChannelSize    = core->coreChannelSize;

    MemoryBlock::copy(info->cmdline, coreInfo.kernelCommand, 64);
    return API::Success;
}
