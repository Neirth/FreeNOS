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

#ifndef __KERNEL_SUPPORT_H
#define __KERNEL_SUPPORT_H

#include <Macros.h>

/**
 * @addtogroup kernel
 * @{
 */

/**
 * Invokes all function pointers inside the .ctors section.
 */
extern C void constructors();

/**
 * Invokes function pointers inside the .dtors section.
 */
extern C void destructors();

/**
 * Unknown function, required by g++.
 */
extern C void __cxa_pure_virtual();

/**
 * Constructor list.
 */
extern void (*CTOR_LIST)();

/**
 * Destructor list.
 */
extern void (*DTOR_LIST)();

/**
 * @}
 */

#endif /* __KERNEL_SUPPORT_H */
