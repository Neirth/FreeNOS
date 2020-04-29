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

#ifndef __LIBARCH_INTEL_STATE_H
#define __LIBARCH_INTEL_STATE_H

#include <Types.h>
#include <Macros.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * Switch Core Stack.
 *
 * This function only needs to save the current Kernel state,
 * and restore the new Kernel state. It will be used by the kernel
 * to preserve its call stack and registers when it wants to switch
 * to a different Process.
 *
 * @param currentStack Address to a pointer to save the current Core state, or ZERO to skip.
 * @param stack Address of the stack to set.
 */
extern C void switchCoreState(Address *currentStack,
                              Address stack);

/**
 * Load Core State.
 *
 * This function is used to restore the full state of the
 * Core. It is used by the kernel as a function to initialize
 * the Process register state when it starts the first time.
 */
extern C void loadCoreState();

/**
 * Interrupt Service Router (ISR) handler.
 *
 * This function saves and restores the complete user
 * state on the kernel stack (found via the TSS).
 * After the kernel has finished executing it will
 * restore the user state completely.
 */
extern C void interruptHandler();

/**
 * Process an interrupt.
 *
 * Callback function which is called by interruptHandler()
 * to process an interrupt.
 *
 * @see interruptHandler
 */
extern C void (*interruptRun)(CPUState state);

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_STATE_H */
