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

#ifndef __ARM_CONTROL_H
#define __ARM_CONTROL_H

#include <Types.h>
#include <Macros.h>

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
 * ARM System Control Coprocessor (CP15).
 *
 * The ARM Architecture defines a coprocessor to
 * manage various subsystems in the ARM core, including
 * virtual memory, interrupts, debugging and more.
 * It appears as coprocessor number 15 to the ARM core
 * and is accessible by the MRC (ARM Register from Coprocessor)
 * and MCR () instructions.
 *
 * @see ARM Architecture Reference Manual, page xxx
 */
class ARMControl
{
  public:

    /**
     * System Control Registers
     */
    enum Register
    {
        MainID = 0,
        CoreID,
        SystemControl,
        AuxControl,
        DomainControl,
        TranslationTable0,
        TranslationTable1,
        TranslationTableCtrl,
        CacheClear,
        DataCacheClean,
        FlushPrefetchBuffer,
        InstructionCacheClear,
        InstructionTLBClear,
        DataTLBClear,
        UnifiedTLBClear,
        UserProcID,
        InstructionFaultAddress,
        InstructionFaultStatus,
        DataFaultAddress,
        DataFaultStatus,
        SystemFrequency
    };

    /**
     * System Control flags.
     */
    enum SystemControlFlags
    {
        MMUEnabled        = (1 << 0),
        InstructionCache  = (1 << 12),
        DataCache         = (1 << 2),
        ExtendedPaging    = (1 << 23),
        AccessPermissions = (1 << 29),
        BranchPrediction  = (1 << 11),
        AlignmentCorrect  = (1 << 22),
        AlignmentFaults   = (1 << 1),
#ifdef ARMV6
        BigEndian         = (1 << 7)
#else
        BigEndian         = (1 << 25)
#endif
    };

    /**
     * Aux Control flags.
     */
    enum AuxControlFlags
    {
        DisablePageColoring = (1 << 6),
        SMPBit              = (1 << 6)
    };

    /**
     * Domain Control flags.
     */
    enum DomainControlFlags
    {
        DomainClient  = 1,
        DomainManager = 3
    };

    /**
     * Constructor
     */
    ARMControl();

    /**
     * Destructor
     */
    virtual ~ARMControl();

    /**
     * Read a register from the CP15.
     *
     * @param reg The Register to read.
     *
     * @return 32-bit value of the register.
     */
    u32 read(Register reg) const;

    /**
     * Write register to the CP15.
     *
     * @param reg The Register to write.
     * @param value 32-value to write.
     */
    void write(Register reg, u32 value);

    /**
     * Set system control flags in CP15.
     *
     * @param flags New system control flags
     */
    void set(SystemControlFlags flags);

    /**
     * Unset system control flags in CP15.
     *
     * @param flags System control flags to remove
     */
    void unset(SystemControlFlags flags);

    /**
     * Set auxillary flags in CP15.
     *
     * @param flags Auxillary flags to set
     */
    void set(AuxControlFlags flags);

    /**
     * Set domain control flags in CP15.
     *
     * @param flags New domain control flags
     */
    void set(DomainControlFlags flags);

  private:

    /**
     * Set flag(s) in a CP15 register.
     *
     * @param reg Register to set
     * @param flags Flag values to set in the register
     */
    void set(Register reg, u32 flags);

    /**
     * Unset flag(s) in a CP15 register.
     *
     * @param reg Register to unset
     * @param flags Flag values to remove from the register
     */
    void unset(Register reg, u32 flags);
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __ARM_CONTROL_H */
