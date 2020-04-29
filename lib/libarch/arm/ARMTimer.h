/*
 * Copyright (C) 2019 Niek Linnenbank
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

#ifndef __LIBARCH_ARM_TIMER_H
#define __LIBARCH_ARM_TIMER_H

#include <Types.h>
#include <Macros.h>
#include <Timer.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * ARM Generic Timer.
 *
 * @note Currently only used for QEMU emulator support of Raspberry Pi
 */
class ARMTimer : public Timer
{
  public:

    ARMTimer();

    /**
     * Set timer frequency.
     *
     * @param hertz Frequency of the timer in hertz.
     *
     * @return Result code
     */
    virtual Result setFrequency(Size hertz);

    /**
     * Process timer tick.
     *
     * Should be called on each Timer interrupt to
     * keep the m_info variable synchronized with the actual hardware.
     * Also clears the timer interrupt flag.
     *
     * @return Result code
     */
    virtual Result tick();

  private:

    /**
     * Retrieve system timer frequency
     *
     * @return System frequency
     */
    u32 getSystemFrequency(void) const;

    /**
     * Set Timer 1 control value
     *
     * @param value New timer control value
     */
    void setPL1Control(u32 value);

    /**
     * Set Physical Timer 1 value
     *
     * @param value New timer value
     */
    void setPL1PhysicalTimerValue(u32 value);

    /**
     * Set Physical Timer 1 control value
     *
     * @param value New timer control value
     */
    void setPL1PhysicalTimerControl(u32 value);

  private:

    Size m_frequency;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_ARMTIMER_H */
