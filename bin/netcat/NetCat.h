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

#ifndef __BIN_NETCAT_H
#define __BIN_NETCAT_H

#include <IPV4.h>
#include <Ethernet.h>
#include <NetworkClient.h>
#include <POSIXApplication.h>

/**
 * @addtogroup bin
 * @{
 */

/**
 * Network send/receive (cat) application.
 */
class NetCat : public POSIXApplication
{
  public:

    /**
     * Class constructor.
     */
    NetCat(int argc, char **argv);

    /**
     * Class destructor.
     */
    virtual ~NetCat();

    /**
     * Initialize the application.
     */
    virtual Result initialize();

    /**
     * Execute the application event loop.
     */
    virtual Result exec();

  private:

    /**
     * Read one line from standard input.
     */
    Result readLine();

    /**
     * Send UDP packet
     */
    Result udpSend();

    Result udpReceive();

    Result printLine();

    /** Networking client */
    NetworkClient *m_client;

    /** Socket */
    int m_socket;

    /** Line buffer */
    char m_lineBuf[64];
    Size m_lineLen;

    /** Host IP */
    IPV4::Address m_host;

    /** Host port */
    u16 m_port;
};

/**
 * @}
 */

#endif /* __BIN_NETCAT_H */
