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

#ifndef __LIBARCH_TIMER_H
#define __LIBARCH_TIMER_H

#include <Macros.h>
#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

/**
 * Represents a configurable timer device.
 */
class Timer
{
  public:

    /**
     * Timer information structure.
     */
    typedef struct Info
    {
        u32 ticks;
        Size frequency;
    }
    ALIGN(8) Info;

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        NotFound,
        IOError,
        InvalidFrequency
    };

    /**
     * Constructor
     */
    Timer();

    /**
     * Get timer interrupt number.
     *
     * @return Interrupt number.
     */
    Size getInterrupt() const;

    /**
     * Get timer frequency.
     *
     * @return Frequency of the timer in hertz.
     */
    Size getFrequency() const;

    /**
     * Set timer frequency.
     *
     * @param hertz Frequency of the timer in hertz.
     *
     * @return Result code.
     */
    virtual Result setFrequency(Size hertz);

    /**
     * Get current timer info.
     *
     * @param info Timer Info object pointer for output.
     *
     * @return Result code.
     */
    virtual Result getCurrent(Info *info);

    /**
     * Initialize the timer.
     *
     * @return Result code.
     */
    virtual Result initialize();

    /**
     * Start the timer.
     *
     * This function lets the timer run. The timer
     * will generate interrupts on the configured frequency.
     *
     * @return Result code.
     */
    virtual Result start();

    /**
     * Stop the timer.
     *
     * This function brings the timer to a halt. The timer
     * will not generate any interrupts.
     *
     * @return Result code.
     */
    virtual Result stop();

    /**
     * Process timer tick.
     *
     * Should be called on each Timer interrupt to
     * keep the m_info variable synchronized with the actual hardware.
     */
    virtual Result tick();

    /**
     * Busy wait a number of microseconds.
     *
     * @param microseconds The number of microseconds to wait at minimum.
     *
     * @return Result code.
     */
    virtual Result wait(u32 microseconds) const;

    /**
     * Check if a timer value is expired
     *
     * @param info Timer info value to compare
     *
     * @return True if expired, false otherwise
     */
    bool isExpired(const Info & info) const;

  protected:

    /** The current Timer information. */
    Info m_info;

    /** Frequency of the Timer. */
    Size m_frequency;

    /** Timer interrupt number. */
    Size m_int;
};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_TIMER_H */
