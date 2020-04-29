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
#include "IOCtl.h"

API::Result IOCtlHandler(IOOperation op, Address addr, ulong value)
{
    Arch::IO io;

    DEBUG("op =" << op << " addr=" << addr << " value=" << value);

    switch (op)
    {
        case IOByteRead:
            return io.inb(addr);

        case IOByteWrite:
            io.outb(addr, value);
            break;

        case IOWordRead:
            return io.inw(addr);

        case IOWordWrite:
            io.outw(addr, value);
            break;

        case IOLongWrite:
            io.outl(addr, value);
            break;
    }
    return API::Success;
}

Log & operator << (Log &log, IOOperation op)
{
    switch (op)
    {
        case IOByteRead:  log.append("IOByteRead");  break;
        case IOByteWrite: log.append("IOByteWrite"); break;
        case IOWordRead:  log.append("IOWordRead");  break;
        case IOWordWrite: log.append("IOWordWrite"); break;
        case IOLongWrite: log.append("IOLongWrite"); break;
    }
    return log;
}
