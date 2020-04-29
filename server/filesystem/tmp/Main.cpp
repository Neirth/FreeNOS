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
#include <KernelLog.h>
#include <stdlib.h>
#include "TmpFileSystem.h"

int main(int argc, char **argv)
{
    SystemInformation info;
    const char *path = "/tmp";

    // Only run on core0
    if (info.coreId != 0)
        return EXIT_SUCCESS;

    KernelLog log;
    log.setMinimumLogLevel(Log::Notice);

    if (argc > 1)
        path = argv[1];

    TmpFileSystem server(path);

    // Mount, then start serving requests.
    server.mount();
    return server.run();
}
