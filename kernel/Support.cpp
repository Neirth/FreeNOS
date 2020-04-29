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

#include <Macros.h>
#include <Assert.h>
#include "Support.h"
#include "Kernel.h"

extern C void constructors()
{
    for (void (**ctor)() = &CTOR_LIST; *ctor; ctor++)
    {
        (*ctor)();
    }
}

extern C void destructors()
{
    for (void (**dtor)() = &DTOR_LIST; *dtor; dtor++)
    {
        (*dtor)();
    }
}

#ifdef __ASSERT__

extern C void __assertFailure(const char *fmt, ...)
{
    for (;;) ;
}

extern C int __assertRead(Address addr)
{

    if (memory && scheduler && scheduler->current())
    {
        return memory->access(scheduler->current(), addr,
                              sizeof(Address), PAGE_PRESENT|PAGE_USER);
    }
    return true;
}

extern C int __assertWrite(Address addr)
{
    if (memory && scheduler && scheduler->current())
    {
        return memory->access(scheduler->current(), addr,
                              sizeof(Address), PAGE_PRESENT|PAGE_USER|PAGE_RW);
    }
    return true;
}

#endif /* __ASSERT__ */

extern C void __cxa_pure_virtual()
{
}

extern C void __dso_handle()
{
}

extern C void __stack_chk_fail(void)
{
}

extern C int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle)
{
    return (0);
}

extern C int raise(int sig)
{
    return 0;
}
