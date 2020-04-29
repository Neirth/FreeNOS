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

#ifndef __KERNEL_PROCESSEVENT_H
#define __KERNEL_PROCESSEVENT_H

#include <Types.h>
#include <Macros.h>
#include "ProcessShares.h"

/**
 * @addtogroup kernel
 * @{
 */

enum ProcessEventType
{
    InterruptEvent,
    ShareCreated,
    ProcessTerminated
};

/**
 * Represents a process which may run on the host.
 */
typedef struct ProcessEvent
{
    ProcessEventType type;
    Size number;
    ProcessShares::MemoryShare share;
}
ProcessEvent;

/**
 * @}
 */

#endif /* __KERNEL_PROCESSEVENT_H */
