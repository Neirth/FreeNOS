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

#include <KernelLog.h>
#include <StdioLog.h>
#include <fcntl.h>
#include <unistd.h>
#include "CoreServer.h"

int main(int argc, char **argv)
{
#if defined(INTEL) && defined(STDIO_LOGGING)
    StdioLog log;
    SystemInformation info;

    // Is this the master core?
    if (info.coreId == 0)
    {
        close(0);
        close(1);
        close(2);

        while (open("/console/tty0", O_RDWR) == -1);
        open("/console/tty0", O_RDWR);
        open("/console/tty0", O_RDWR);
    }
#else
    KernelLog log;
#endif

    log.setMinimumLogLevel(Log::Notice);
    NOTICE("initializing on core0");

    CoreServer server;
    server.initialize();
    server.test();
    return server.runCore();
}
