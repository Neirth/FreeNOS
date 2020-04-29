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

#ifndef __LIBALLOC_ALLOCATOR_H
#define __LIBALLOC_ALLOCATOR_H
#ifndef __ASSEMBLER__

#include <Macros.h>
#include <Types.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnew-returns-null"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup liballoc
 * @{
 */

/**
 * Memory Allocator.
 *
 * This class defines an abstract memory allocator. Each
 * class which derives must provide functions for memory
 * allocation and release. The memory allocators form a hierarcy
 * of parent-child. A parent allocator can provide memory to a child
 * allocator. If a child allocator runs out of memory, it can ask the
 * parent for more memory.
 */
class Allocator
{
  public:

    /**
     * Allocation results.
     */
    enum Result
    {
        Success = 0,
        InvalidAddress,
        InvalidSize,
        InvalidAlignment,
        OutOfMemory
    };

    /**
     * Allocator input/output arguments
     */
    typedef struct Arguments
    {
        /** Output parameter which contains the address allocated on success. */
        Address address;

        /**
         * Amount of memory in bytes.
         * On input this member specified the number of bytes to allocate.
         * On output, represents the actual amount of allocated memory in bytes.
         */
        Size size;

        /** Alignment of the required memory or use ZERO for default alignment. */
        Size alignment;

    } Arguments;

  public:

    /**
     * Class constructor.
     */
    Allocator();

    /**
     * Class destructor.
     */
    virtual ~Allocator();

    /**
     * Makes the given Allocator the default.
     *
     * @param alloc Instance of an Allocator.
     */
    static void setDefault(Allocator *alloc);

    /**
     * Retrieve the currently default Allocator.
     *
     * @return Allocator pointer.
     */
    static Allocator *getDefault();

    /**
     * Set parent allocator.
     *
     * @param p New parent allocator.
     */
    void setParent(Allocator *parent);

    /**
     * Set allocation alignment.
     *
     * Configure the Allocator such that each allocated
     * address must be aligned to the given size.
     *
     * @param size Alignment size
     *
     * @return Result code
     */
    Result setAlignment(Size size);

    /**
     * Set allocation base.
     *
     * The allocation base will be added to each allocation.
     *
     * @param addr Allocation base address.
     *
     * @return Result code
     */
    Result setBase(Address addr);

    /**
     * Get memory size.
     *
     * @return Size of memory owned by the Allocator.
     */
    virtual Size size() const = 0;

    /**
     * Get memory available.
     *
     * @return Size of memory available by the Allocator.
     */
    virtual Size available() const = 0;

    /**
     * Allocate memory.
     *
     * @param args Allocator arguments containing the requested size, address and alignment.
     *
     * @return Result value.
     */
    virtual Result allocate(Arguments & args) = 0;

    /**
     * Release memory.
     *
     * @param addr Points to memory previously returned by allocate().
     *
     * @return Result value.
     *
     * @see allocate
     */
    virtual Result release(Address addr) = 0;

  protected:

    /**
     * Align memory address.
     *
     * Any alignment corrections on the input address will result
     * in an address which is higher than the input address.
     *
     * @param addr Input address which need to be aligned.
     * @param boundary Boundary size to align the address for.
     *
     * @return Aligned Address.
     */
    Address aligned(Address addr, Size boundary) const;

  protected:

    /** Our parent Allocator, if any. */
    Allocator *m_parent;

    /** Allocation memory alignment. */
    Size m_alignment;

    /** Allocation base address */
    Address m_base;

  private:

    /** Points to the default Allocator for new()/delete(). */
    static Allocator *m_default; 
};

#ifndef __HOST__

/**
 * @name Dynamic memory allocation.
 * @{
 */

/**
 * Allocate new memory.
 *
 * @param sz Amount of memory to allocate.
 */
inline void * operator new(__SIZE_TYPE__ sz)
{
    Allocator::Arguments alloc_args;

    alloc_args.size = sz;
    alloc_args.address = 0;
    alloc_args.alignment = 0;

    if (Allocator::getDefault()->allocate(alloc_args) == Allocator::Success)
        return (void *) alloc_args.address;
    else
        return (void *) NULL;
}

/**
 * Allocate memory for an array.
 *
 * @param sz Amount of memory to allocate.
 */
inline void * operator new[](__SIZE_TYPE__ sz)
{
    Allocator::Arguments alloc_args;

    alloc_args.size = sz;
    alloc_args.address = 0;
    alloc_args.alignment = 0;

    if (Allocator::getDefault()->allocate(alloc_args) == Allocator::Success)
        return (void *) alloc_args.address;
    else
        return (void *) NULL;
}

/**
 * Free memory back to the current Allocator.
 *
 * @param mem Points to memory to release.
 */
inline void operator delete (void *mem)
{
    Allocator::getDefault()->release((Address)mem);
}

/**
 * Uses the Heap class to free memory, with the delete[] operator.
 *
 * @param mem Points to memory to release.
 */
inline void operator delete[] (void *mem)
{
    Allocator::getDefault()->release((Address)mem);
}

/**
 * Let the new() operator return the given memory address.
 *
 * @param sz Size to allocate (ignored).
 * @param addr Memory address to return.
 */
inline void * operator new(__SIZE_TYPE__ sz, Address addr)
{
    return (void *) addr;
}

/**
 * @}
 */

#endif /* __HOST__ */

/**
 * @}
 * @}
 */

#endif /* __ASSEMBLER__ */
#endif /* __LIBALLOC_ALLOCATOR_H */
