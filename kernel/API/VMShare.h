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

#ifndef __API_VMSHARE_H
#define __API_VMSHARE_H

#include <FreeNOS/System.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernelapi
 * @{
 */

/**
 * Prototype for user applications. Creates and removes shared virtual memory mappings.
 *
 * @param op Determines which operation to perform.
 * @param pid Remote process.
 * @param parameter Parameter for the operation.
 * @param size Size parameter for the operation.
 *
 * @return API::Success on success and other API::ErrorCode on failure.
 */
inline API::Result VMShare(ProcessID pid, API::Operation op, ProcessShares::MemoryShare *share)
{
    return trapKernel3(API::VMShareNumber, pid, op, (Address) share);
}

/**
 * @}
 */

#ifdef __KERNEL__

/**
 * @addtogroup kernelapi_handler
 * @{
 */

extern API::Result VMShareHandler(ProcessID pid, API::Operation op, ProcessShares::MemoryShare *share);

/**
 * @}
 */

#endif /* __KERNEL__ */

/**
 * @}
 */

#endif /* __API_VMSHARE_H */
