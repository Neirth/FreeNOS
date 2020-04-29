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

#include <FreeNOS/Config.h>
#include <string.h>
#include "unistd.h"
#include "sys/utsname.h"

int uname(struct utsname *name)
{
    // Fill in the struct
    strlcpy(name->sysname, "FreeNOS", UTSBUF);
    gethostname(name->nodename, UTSBUF);
    strlcpy(name->release, RELEASE, UTSBUF);
    strlcpy(name->version, COMPILER_VERSION " " DATETIME, UTSBUF);
    strlcpy(name->machine, ARCH "/" SYSTEM, UTSBUF);

    // Success
    return 0;
}
