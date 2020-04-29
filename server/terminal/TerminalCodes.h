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

#ifndef __TERMINAL_CODES_H
#define __TERMINAL_CODES_H

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup terminal
 * @{
 */

/** Set Graphic Rendition. */
#define _SGR(x) "\033[" #x "m"

/**
 * @name Terminal Colors
 * @see http://en.wikipedia.org/wiki/ANSI_escape_code
 * @{
 */

#define BLACK   _SGR(30)
#define RED     _SGR(31)
#define GREEN   _SGR(32)
#define YELLOW  _SGR(33)
#define BLUE    _SGR(34)
#define MAGENTA _SGR(35)
#define CYAN    _SGR(36)
#define WHITE   _SGR(37)
#define RESET   _SGR(39)

/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif /* __TERMINAL_CODES_H */
