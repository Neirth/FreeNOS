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

#ifndef __LIB_LIBFS_DEVICESERVER_H
#define __LIB_LIBFS_DEVICESERVER_H

#include "FileSystem.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/** Maximum number of devices handled simultaneously by a DeviceServer. */
#define DEVICE_MAX 32

/**
 * @brief Device driver server.
 *
 * Implements IPC handlers to communicate with the FileSystem using
 * FileSystemMessages and invokes Device functions appropriately.
 *
 * @see Device
 * @see FileSystem
 * @see FileSystemMessage
 */
class DeviceServer : public FileSystem
{
  public:

    /**
     * Constructor
     */
    DeviceServer(const char *path);

    /**
     * Destructor
     */
    virtual ~DeviceServer();

    /**
     * Initialize DeviceServer.
     *
     * @return Error code
     */
    virtual Error initialize();

    /**
     * @brief Add a Device.
     *
     * Adds an Device to the internal Vector of Devices, to
     * be able to process requests for the Device later on.
     *
     * @param dev New device to add
     * @param path Path format to the device
     * @param ... Path format arguments
     */
    void registerDevice(Device *dev, const char *path, ...);

    /**
     * @brief Register an interrupt vector for the given device.
     *
     * Appends the given Device on the internal list used
     * for interrupt processing in interruptHandler().
     *
     * @param dev Pointer to the Device to wait interrupts for.
     * @param vector Vector number of the interrupt.
     *
     * @see Device
     * @see interruptHandler
     */
    void registerInterrupt(Device *dev, Size vector);

    /**
     * @brief Interrupt request handler.
     *
     * Invokes the interrupt callback function of
     * each Device registered for the interrupt vector.
     *
     * @param vector Interrupt number
     * @see Device
     * @see Device::interrupt
     */
    virtual void interruptHandler(Size vector);

  private:

    /** Contains all Devices served by this DeviceServer. */
    Vector<Device *> m_devices;

    /**
     * @brief Registers Devices using interrupts.
     *
     * An Vector with Lists of Devices using the
     * interrupt vector as index.
     *
     * @see Vector
     * @see List
     * @see Device
     */
    Vector<List<Device *> *> m_interrupts;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_DEVICESERVER_H */
