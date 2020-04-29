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

#ifndef __KERNEL_API_H
#define __KERNEL_API_H

#ifndef __SYSTEM
#error Do not include FreeNOS/API.h directly, use FreeNOS/System.h instead
#endif

#include <Log.h>
#include <Types.h>
#include <Vector.h>

/**
 * @addtogroup kernel
 * @{
 */

/**
 * Generic Kernel API implementation.
 */
class API
{
  public:

    /**
     * Enumeration of supported generic kernel API functions.
     *
     * Architectures or System implementations can optionally
     * introduce additional specific APIs.
     */
    typedef enum Number
    {
        PrivExecNumber = 1,
        ProcessCtlNumber,
        SystemInfoNumber,
        VMCopyNumber,
        VMCtlNumber,
        VMShareNumber,
        IOCtlNumber
    }
    Number;

    /**
     * Enumeration of generic kernel API error codes.
     */
    typedef enum ErrorCode
    {
        Success         =  0,
        AccessViolation = -1,
        RangeError      = -2,
        NotFound        = -3,
        InvalidArgument = -4,
        OutOfMemory     = -5,
        IOError         = -6,
        AlreadyExists   = -7
    }
    Error;

    /** Represents the result of an system call handler */
    typedef ::Error Result;

    /**
     * Function which handles an kernel API (system call) request.
     * @return Status code of the APIHandler execution.
     */
    typedef Result Handler(ulong, ulong, ulong, ulong, ulong);

    /**
     * Various actions which may be performed inside an APIHandler.
     */
    typedef enum Operation
    {
        Create      = 0,
        Delete      = 1,
        Send        = 2,
        Receive     = 3,
        SendReceive = 4,
        Read        = 5,
        Write       = 6,
        ReadPhys    = 7
    }
    Operation;

  public:

    /**
     * Constructor
     */
    API();

    /**
     * Execute a generic API function.
     */
    Result invoke(Number number,
                  ulong arg1,
                  ulong arg2,
                  ulong arg3,
                  ulong arg4,
                  ulong arg5);

  private:

    /** API handlers */
    Vector<Handler *> m_apis;
};

/**
 * @}
 */

/** Operator to print a Operation to a Log */
Log & operator << (Log &log, API::Operation op);

/*
 * Include generic kernel API functions.
 */

#include "API/PrivExec.h"
#include "API/ProcessCtl.h"
#include "API/SystemInfo.h"
#include "API/VMCopy.h"
#include "API/VMCtl.h"
#include "API/VMShare.h"
#include "API/IOCtl.h"
#include "API/ProcessID.h"

#endif /* __KERNEL_API_H */
