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

#ifndef __API_VMCOPY_H
#define __API_VMCOPY_H

#include <FreeNOS/System.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernelapi
 * @{
 */

/**
 * Prototype for user applications. Copies virtual memory between two processes.
 *
 * @param proc Remote process.
 * @param how Read or Write.
 * @param ours Virtual address of the buffer of this process.
 * @param theirs Virtual address of the remote process' buffer.
 * @param sz Amount of memory to copy.
 *
 * @return Total number of bytes copied on success and API::ErrorCode on failure.
 */
inline API::Result VMCopy(ProcessID proc, API::Operation how, Address ours,
                          Address theirs, Size sz)
{
    return trapKernel5(API::VMCopyNumber, proc, how, ours, theirs, sz);
}

/**
 * @}
 */

#ifdef __KERNEL__

/**
 * @addtogroup kernelapi_handler
 * @{
 */

extern API::Result VMCopyHandler(ProcessID proc, API::Operation how, Address ours, Address theirs, Size sz);

/**
 * @}
 */

#endif /* __KERNEL__ */

/**
 * @}
 */

#endif /* __API_VMCOPY_H */
