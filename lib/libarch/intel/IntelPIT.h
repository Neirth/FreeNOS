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

#ifndef __LIBARCH_INTEL_PIT_H
#define __LIBARCH_INTEL_PIT_H

#include <Types.h>
#include <BitOperations.h>
#include <Timer.h>
#include "IntelIO.h"

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
 * Intel 8254 Programmable Interrupt Timer (PIT).
 */
class IntelPIT : public Timer
{
  private:

    /** Oscillator frequency in hertz used by the PIT. */
    static const uint OscillatorFreq = 1193182;

    /** The IRQ vector for channel 0 is fixed to IRQ0 */
    static const uint InterruptNumber = 0;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        Control      = 0x43,
        Channel0Data = 0x40
    };

    /**
     * Control Register Flags.
     */
    enum ControlFlags
    {
        Channel0      = 0,
        LatchedRead   = 0,
        AccessLowHigh = (3 << 4),
        SquareWave    = (3 << 1),
        RateGenerator = (2 << 1),
    };

  public:

    /**
     * Constructor
     */
    IntelPIT();

    /**
     * Get current timer counter.
     *
     * The timer counter is used internally by the PIT
     * to generate interrupts on the desired frequency.
     * The PIT will decrement the timer counter until 0
     * is reached, at which it will generate an interrupt
     * and restart at the initial timer counter value.
     *
     * @return Current timer counter.
     */
    uint getCounter();

    /**
     * Set interrupt frequency.
     *
     * This function configures the PIT
     * to generate interrupts on the given frequency.
     * The new frequency will be applied immediately on
     * the interrupts generated.
     *
     * @param hertz Number of interrupt triggers per second (in hertz)
     *
     * @return Result code.
     */
    virtual Result setFrequency(Size hertz);

    /**
     * Busy wait for one trigger period.
     *
     * @return Result code.
     */
    Result waitTrigger();

  private:

    /**
     * Set Control register.
     *
     * @param flags New flags to set in the Control register.
     *
     * @return Result code.
     */
    Result setControl(ControlFlags flags);

  private:

    /** I/O instance */
    IntelIO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_PIT_H */
