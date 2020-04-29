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

#ifndef __LIBNET_ETHERNETADDRESS_H
#define __LIBNET_ETHERNETADDRESS_H

#include <Types.h>
#include "File.h"
#include "Ethernet.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Ethernet address file.
 */
class EthernetAddress : public File
{
  public:

    /**
     * Constructor
     */
    EthernetAddress(Ethernet *eth);

    /**
     * Destructor
     */
    virtual ~EthernetAddress();

    /**
     * Read Ethernet address
     *
     * @param buffer Input/Output buffer to output bytes to.
     * @param size Number of bytes to read, at maximum.
     * @param offset Offset inside the file to start reading.
     * @return Number of bytes read on success, Error on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Set new Ethernet address
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Number of bytes to write, at maximum.
     * @param offset Offset inside the file to start writing.
     * @return Number of bytes written on success, Error on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Ethernet object pointer */
    Ethernet *m_eth;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_ETHERNETADDRESS_H */
