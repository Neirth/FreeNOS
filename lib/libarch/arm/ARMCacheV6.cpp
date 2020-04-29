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

#include "ARMCore.h"
#include "ARMCacheV6.h"

ARMCacheV6::Result ARMCacheV6::invalidate(ARMCacheV6::Type type)
{
    switch (type)
    {
        case Instruction:
            return cleanInvalidate(type);

        case Data:
            mcr(p15, 0, 0, c7, c6, 0);
            dsb();
            break;

        case Unified:
            invalidate(Instruction);
            invalidate(Data);
            break;
    }
    return Success;
}

ARMCacheV6::Result ARMCacheV6::cleanInvalidate(ARMCacheV6::Type type)
{
    switch (type)
    {
        case Instruction:
            //
            // Invalidate all instruction caches to PoU.
            // Also flushes branch target cache.
            //
            mcr(p15, 0, 0, c7, c5, 0);

            // Invalidate entire branch predictor array
            flushBranchPrediction();

            // Full system DSB - make sure that the invalidation is complete
            dsb();

            // Make sure the instruction stream sees it
            flushPrefetchBuffer();
            break;

        case Data:
            mcr(p15, 0, 0, c7, c14, 0);
            break;

        case Unified:
            mcr(p15, 0, 0, c7, c5,  0);    // invalidate entire instruction cache
            mcr(p15, 0, 0, c7, c10, 0);    // clean entire data cache
            mcr(p15, 0, 0, c7, c7,  0);    // invalidate entire cache
            flushBranchPrediction();
            dsb();
            flushPrefetchBuffer();
            break;
    }
    return Success;
}

ARMCacheV6::Result ARMCacheV6::cleanInvalidateAddress(Type type, Address addr)
{
    switch (type)
    {
        case Instruction:
            mcr(p15, 0, 1, c7, c5, addr);
            break;

        case Data:
            mcr(p15, 0, 1, c7, c14, addr);
            break;

        case Unified:
            break;
    }
    return Success;
}

ARMCacheV6::Result ARMCacheV6::cleanAddress(ARMCacheV6::Type type, Address addr)
{
    switch (type)
    {
        case Instruction:
            break;

        case Data:
            mcr(p15, 0, 1, c7, c10, addr);
            break;

        case Unified:
            break;
    }
    return Success;
}
