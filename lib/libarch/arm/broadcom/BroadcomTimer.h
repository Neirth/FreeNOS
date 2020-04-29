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

#ifndef __LIBARCH_ARM_BROADCOMTIMER_H
#define __LIBARCH_ARM_BROADCOMTIMER_H

#include <Types.h>
#include <Macros.h>
#include <Timer.h>
#include <arm/ARMIO.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_bcm
 * @{
 */

#define SYSTIMER_BASE 0x3000

#define SYSTIMER_CS   (SYSTIMER_BASE + 0x0)
#define SYSTIMER_CLO  (SYSTIMER_BASE + 0x4)

/** Used by GPU */
#define SYSTIMER_C0   (SYSTIMER_BASE + 0xC)
#define SYSTIMER_C1   (SYSTIMER_BASE + 0x10)

/** Used by GPU */
#define SYSTIMER_C2   (SYSTIMER_BASE + 0x14)
#define SYSTIMER_C3   (SYSTIMER_BASE + 0x18)
    #define M3 3
    #define M2 2
    #define M1 1
    #define M0 0

/** BCM Sys Timer runs at 1Mhz */
#define BCM_SYSTIMER_FREQ 1000000

/**
 * Broadcom System-on-Chip system timer.
 */
class BroadcomTimer : public Timer
{
  public:

    /**
     * Set timer frequency.
     *
     * @param hertz Frequency of the timer in hertz.
     *
     * @return Result code.
     */
    virtual Result setFrequency(Size hertz);

    /**
     * Process timer tick.
     *
     * Should be called on each Timer interrupt to
     * keep the m_info variable synchronized with the actual hardware.
     * Also clears the timer interrupt flag.
     *
     * @return Result code.
     */
    virtual Result tick();

  private:

    /** Number of internal cycles needed to provide the current timer frequency */
    u32 m_cycles;

    /** I/O instance */
    ARMIO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_BROADCOMTIMER_H */
