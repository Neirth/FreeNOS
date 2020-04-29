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

#ifndef __LIB_LIBFS_DEVICELOG_H
#define __LIB_LIBFS_DEVICELOG_H

#include <Log.h>
#include <Types.h>
#include "Device.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Generic logger that writes to a Device object
 *
 * @see Device
 */
class DeviceLog : public Log
{
  public:

    /**
     * Constructor.
     *
     * @param device Device instance to write log messages to
     */
    DeviceLog(Device & device);

  protected:

    /**
     * Write a string to the Device one character at a time
     *
     * @param str NUL-terminated string
     */
    virtual void write(const char *str);

  private:

    /** Device instance */
    Device & m_device;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_DEVICELOG_H */
