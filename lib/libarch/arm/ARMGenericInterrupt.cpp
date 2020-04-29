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

#include <Log.h>
#include "ARMGenericInterrupt.h"

ARMGenericInterrupt::ARMGenericInterrupt(
    Address distRegisterBase,
    Address cpuRegisterBase)
    : IntController()
{
    // Set I/O register bases
    m_dist.setBase(distRegisterBase);
    m_cpu.setBase(cpuRegisterBase);

    // Read number of interrupt lines available
    m_numIrqs = 32 * ((m_dist.read(GICD_TYPER) & DistTypeIrqsMask) + 1);
    NOTICE(m_numIrqs << " IRQ lines");

    // Set all interrupts in group 0 and disable all interrupts
    for (uint i = 0; i < numRegisters(1); i++)
    {
        m_dist.write(GICD_GROUPR + (i * 4), 0);
        m_dist.write(GICD_ICENABLER + (i * 4), ~0);
    }

    // Set interrupt configuration to level triggered (2-bits)
    for (uint i = 0; i < (m_numIrqs / 2); i++)
    {
        m_dist.write(GICD_ICFGR + (i * 4), 0);
    }

    // Set interrupt priority
    for (uint i = 0; i < (m_numIrqs / 4); i++)
    {
        m_dist.write(GICD_IPRIORITYR + (i * 4), 0xA0A0A0A0);
    }

    // Set minimum priority level
    m_cpu.write(GICC_PMR, 0xF0);

    // All interrupts are assigned to core0.
    // Ignore the first 32 PPIs, which are local and banked per core.
    for (uint i = 8; i < (m_numIrqs / 4); i++)
    {
        m_dist.write(GICD_ITARGETSR + (i * 4),
                    (1 << 0) | (1 << 8) | (1 << 16) | (1 << 24));
    }

    // Enable all groups
    m_dist.write(GICD_CTRL, DistCtrlGroup0 | DistCtrlGroup1);
    m_cpu.write(GICC_CTRL, CpuCtrlGroup0 | CpuCtrlGroup1);
}

ARMGenericInterrupt::Result ARMGenericInterrupt::enable(uint irq)
{
    m_dist.set(GICD_ISENABLER + (4 * (irq / 32)), (1 << (irq % 32)));
    return Success;
}

ARMGenericInterrupt::Result ARMGenericInterrupt::disable(uint irq)
{
    m_dist.set(GICD_ICENABLER + (4 * (irq / 32)), (1 << (irq % 32)));
    return Success;
}

ARMGenericInterrupt::Result ARMGenericInterrupt::clear(uint irq)
{
    m_cpu.write(GICC_EOIR, irq);
    m_cpu.write(GICC_DIR, irq);
    return Success;
}

ARMGenericInterrupt::Result ARMGenericInterrupt::nextPending(uint & irq)
{
    irq = m_cpu.read(GICC_IAR) & CpuIrqAckMask;

    // Is this a spurious (unexpected) interrupt?
    if (irq == 1023)
    {
        return NotFound;
    }
    else
        return Success;
}

bool ARMGenericInterrupt::isTriggered(uint irq)
{
    // Unused
    return false;
}

Size ARMGenericInterrupt::numRegisters(Size bits) const
{
    if (m_numIrqs % 32)
        return ((m_numIrqs * bits) / 32) + 1;
    else
        return (m_numIrqs * bits) / 32;
}
