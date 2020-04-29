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

/*
 * Partly based on cache_v7.c from the Android project:
 *
 *  https://android.googlesource.com/device/ti/bootloader/uboot/+/c0eec2d5698a6722a195f4545064dccfb4010c16/arch/arm/cpu/armv7/cache_v7.c
 */

#include "ARMCore.h"
#include "ARMCacheV7.h"

#define CCSIDR_LINE_SIZE_OFFSET          0
#define CCSIDR_LINE_SIZE_MASK            0x7
#define CCSIDR_ASSOCIATIVITY_OFFSET      3
#define CCSIDR_ASSOCIATIVITY_MASK        (0x3FF << 3)
#define CCSIDR_NUM_SETS_OFFSET           13
#define CCSIDR_NUM_SETS_MASK             (0x7FFF << 13)

ARMCacheV7::Result ARMCacheV7::invalidate(ARMCacheV7::Type type)
{
    switch (type)
    {
        case Instruction:
            return cleanInvalidate(type);

        case Data:
            return dataFlush(false);

        case Unified:
            invalidate(Instruction);
            invalidate(Data);
            break;
    }
    return Success;
}

ARMCacheV7::Result ARMCacheV7::cleanInvalidate(ARMCacheV7::Type type)
{
    switch (type)
    {
        case Instruction:
            //
            // Invalidate all instruction caches to PoU.
            // Also flushes branch target cache.
            //
            dsb();
            mcr(p15, 0, 0, c7, c5, 0);

            // Invalidate entire branch predictor array
            flushBranchPrediction();
            isb();
            break;

        case Data:
            dataFlush(true);
            dsb();
            break;

        case Unified:
            cleanInvalidate(Data);
            cleanInvalidate(Instruction);
            break;
    }
    return Success;
}

ARMCacheV7::Result ARMCacheV7::cleanInvalidateAddress(Type type, Address addr)
{
    switch (type)
    {
        case Instruction:
            mcr(p15, 0, 1, c7, c5, addr);
            isb();
            break;

        case Data:
            mcr(p15, 0, 1, c7, c14, addr);
            dsb();
            break;

        case Unified:
            break;
    }
    return Success;
}

ARMCacheV7::Result ARMCacheV7::cleanAddress(ARMCacheV7::Type type, Address addr)
{
    switch (type)
    {
        case Instruction:
            mcr(p15, 0, 1, c7,  c5, addr);
            isb();
            break;

        case Data:
            mcr(p15, 0, 1, c7, c10, addr);
            dsb();
            isb();
            break;

        case Unified:
            break;
    }
    return Success;
}

u32 ARMCacheV7::getCacheLevelId() const
{
    u32 levelId;
    asm volatile ("mrc p15,1,%0,c0,c0,1" : "=r" (levelId));
    return levelId;
}

u32 ARMCacheV7::readCacheSize(u32 level, u32 type) const
{
    u32 sel = level << 1 | type;
    u32 ids;

    // Cache Size Selection Register
    asm volatile ("mcr p15, 2, %0, c0, c0, 0" : : "r" (sel));

    // Current Cache Size ID Register
    asm volatile ("mrc p15, 1, %0, c0, c0, 0" : "=r" (ids));
    return ids;
}

static inline s32 log_2_n_round_up(u32 n)
{
    s32 log2n = -1;
    u32 temp = n;

    while (temp) {
        log2n++;
        temp >>= 1;
    }

    if (n & (n - 1))
        return log2n + 1; // not power of 2 - round up
    else
        return log2n; // power of 2
}


ARMCacheV7::Result ARMCacheV7::flushLevel(u32 level, bool clean)
{
    u32 ccsidr = readCacheSize(level, 0);
    int way, set, setway;
    u32 log2_line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>
                                   CCSIDR_LINE_SIZE_OFFSET) + 2;

    // Converting from words to bytes
    log2_line_len += 2;

    u32 num_ways  = ((ccsidr & CCSIDR_ASSOCIATIVITY_MASK) >>
                               CCSIDR_ASSOCIATIVITY_OFFSET) + 1;
    u32 num_sets  = ((ccsidr & CCSIDR_NUM_SETS_MASK) >>
                               CCSIDR_NUM_SETS_OFFSET) + 1;
    //
    // According to ARMv7 ARM number of sets and number of ways need
    // not be a power of 2
    //
    u32 log2_num_ways = ::log_2_n_round_up(num_ways);
    u32 way_shift     = (32 - log2_num_ways);

    // Invoke the Clean & Invalidate cache line by set/way on the CP15.
    for (way = num_ways - 1; way >= 0 ; way--)
    {
        for (set = num_sets - 1; set >= 0; set--)
        {
            setway = (level << 1) | (set << log2_line_len) |
                     (way << way_shift);
            //
            // Clean & Invalidate data/unified
            // cache line by set/way
            //
            if (clean)
            {
                asm volatile (" mcr p15, 0, %0, c7, c14, 2"
                                : : "r" (setway));
            }
            else
            {
                asm volatile (" mcr p15, 0, %0, c7, c6, 2"
                                : :  "r" (setway));
            }
        }
    }
    // Data Synchronisation Barrier to ensure operations are complete
    dsb();
    return Success;
}

ARMCacheV7::Result ARMCacheV7::dataFlush(bool clean)
{
    u32 levelId = getCacheLevelId();
    u32 cacheType, startBit = 0;

    for (u32 level = 0; level < 7; level++)
    {
        cacheType = (levelId >> startBit) & 7;
        if (cacheType == CacheLevelData ||
            cacheType == CacheLevelInstruction ||
            cacheType == CacheLevelUnified)
        {
            flushLevel(level, clean);
        }
        startBit += 3;
    }
    return Success;
}

