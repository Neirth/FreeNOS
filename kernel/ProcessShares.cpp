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

#include <FreeNOS/System.h>
#include <MemoryChannel.h>
#include <List.h>
#include <ListIterator.h>
#include <SplitAllocator.h>
#include "ProcessEvent.h"

ProcessShares::ProcessShares(ProcessID pid)
{
    m_pid    = pid;
    m_memory = ZERO;
    m_kernelChannel = new MemoryChannel;
}

ProcessShares::~ProcessShares()
{
    ProcessManager *procs = Kernel::instance->getProcessManager();
    List<ProcessID> pids;

    // Cleanup members
    delete m_kernelChannel;

    // Make a list of unique process IDs which
    // have a share with this Process
    Size size = m_shares.size();
    for (Size i = 0; i < size; i++)
    {
        MemoryShare *sh = (MemoryShare *) m_shares.get(i);
        if (sh)
        {
            if (!pids.contains(sh->pid))
                pids.append(sh->pid);

            releaseShare(sh, i);
        }
    }

    // Raise process terminated events
    for (ListIterator<ProcessID> i(pids); i.hasCurrent(); i++)
    {
        ProcessID pid = i.current();
        Process *proc = procs->get(pid);
        if (proc)
        {
            ProcessEvent event;
            event.type = ProcessTerminated;
            event.number = m_pid;
            procs->raiseEvent(proc, &event);
        }
    }
}

const ProcessID ProcessShares::getProcessID() const
{
    return m_pid;
}

MemoryContext * ProcessShares::getMemoryContext()
{
    return m_memory;
}

ProcessShares::Result ProcessShares::setMemoryContext(MemoryContext *ctx)
{
    m_memory = ctx;
    return Success;
}

ProcessShares::Result ProcessShares::createShare(ProcessID pid,
                                                 Size coreId,
                                                 Size tagId,
                                                 Address virt,
                                                 Size size)
{
    MemoryShare *share = ZERO;

    if (size == 0 || size % PAGESIZE)
        return InvalidArgument;

    // Allocate MemoryShare objects
    share  = new MemoryShare;
    if (!share)
    {
        ERROR("failed to allocate MemoryShare");
        return OutOfMemory;
    }

    // Fill the share object
    share->pid        = pid;
    share->coreId     = coreId;
    share->tagId      = tagId;
    share->range.virt = virt;
    share->range.size = size;
    m_memory->lookup(share->range.virt, &share->range.phys);
    m_memory->access(share->range.virt, &share->range.access);

    // insert into shares list
    m_shares.insert(*share);
    return Success;
}

ProcessShares::Result ProcessShares::createShare(ProcessShares & instance,
                                                 ProcessShares::MemoryShare *share)
{
    MemoryShare *localShare = ZERO;
    MemoryShare *remoteShare = ZERO;
    MemoryContext *localMem  = m_memory;
    MemoryContext *remoteMem = instance.getMemoryContext();
    Address paddr, vaddr;
    Arch::Cache cache;
    Allocator::Arguments alloc_args;

    if (share->range.size == 0)
        return InvalidArgument;

    // Check if the share already exists
    if (readShare(share) == Success)
        return AlreadyExists;

    // Allocate local
    localShare = new MemoryShare;
    if (!localShare)
    {
        ERROR("failed to allocate MemoryShare for local process");
        return OutOfMemory;
    }

    // Allocate remote
    remoteShare = new MemoryShare;
    if (!remoteShare)
    {
        ERROR("failed to allocate MemoryShare for remote process");
        delete localShare;
        return OutOfMemory;
    }

    // Allocate actual pages
    alloc_args.address = 0;
    alloc_args.size = share->range.size;
    alloc_args.alignment = PAGESIZE;

    if (Kernel::instance->getAllocator()->allocateLow(alloc_args) != Allocator::Success)
    {
        ERROR("failed to allocate pages for MemoryShare");
        return OutOfMemory;
    }
    paddr = alloc_args.address;

    // Zero out the pages
    vaddr = (Address) Kernel::instance->getAllocator()->toVirtual(paddr);
    MemoryBlock::set((void *)vaddr, 0, share->range.size);
    for (Size i = 0; i < share->range.size; i+=PAGESIZE)
        cache.cleanData(vaddr + i);

    // Fill the local share object
    localShare->pid        = instance.getProcessID();
    localShare->coreId     = Kernel::instance->getCoreInfo()->coreId;
    localShare->tagId      = share->tagId;
    localShare->range.phys = paddr;
    localShare->range.size = share->range.size;
    localShare->range.access = Memory::User | share->range.access;
    localShare->attached   = true;

    // Map in the local process
    if (localMem->findFree(localShare->range.size, MemoryMap::UserShare, &localShare->range.virt) != MemoryContext::Success ||
        localMem->mapRange(&localShare->range) != MemoryContext::Success)
    {
        ERROR("failed to map MemoryShare in local process");
        delete localShare;
        delete remoteShare;
        return OutOfMemory;
    }
    // Fill the remote share object
    remoteShare->pid          = m_pid;
    remoteShare->coreId       = localShare->coreId;
    remoteShare->tagId        = localShare->tagId;
    remoteShare->range.phys   = localShare->range.phys;
    remoteShare->range.size   = localShare->range.size;
    remoteShare->range.access = localShare->range.access;
    remoteShare->attached     = true;

    // Map in the remote process
    if (remoteMem->findFree(remoteShare->range.size, MemoryMap::UserShare, &remoteShare->range.virt) != MemoryContext::Success ||
        remoteMem->mapRange(&remoteShare->range) != MemoryContext::Success)
    {
        ERROR("failed to map MemoryShare in remote process");
        delete localShare;
        delete remoteShare;
        return OutOfMemory;
    }
    // insert into shares list
    m_shares.insert(*localShare);
    instance.m_shares.insert(*remoteShare);

    // raise event on the remote process
    ProcessManager *procs = Kernel::instance->getProcessManager();
    Process *proc = procs->get(instance.getProcessID());
    ProcessEvent event;
    event.type   = ShareCreated;
    event.number = m_pid;
    MemoryBlock::copy(&event.share, remoteShare, sizeof(*remoteShare));
    procs->raiseEvent(proc, &event);

    // Update parameter outputs
    MemoryBlock::copy(share, localShare, sizeof(*share));
    return Success;
}

ProcessShares::Result ProcessShares::removeShares(ProcessID pid)
{
    Size size = m_shares.size();
    MemoryShare *s = 0;

    for (Size i = 0; i < size; i++)
    {
        if ((s = (MemoryShare *) m_shares.get(i)) != ZERO)
        {
            if (s->pid != pid)
                continue;

            releaseShare(s, i);
        }
    }
    return Success;
}

ProcessShares::Result ProcessShares::releaseShare(MemoryShare *s, Size idx)
{
    // Only release physical memory if both processes have detached.
    // Note that in case all memory shares for a certain ProcessID have
    // been detached but not yet released, and due to a very unlikely race
    // condition (ProcessID reuse) a new memory share was just created (before old ones were released)
    // the new memory share would also be detached here, resulting in a memory
    // share with is detached in this process but attached and useless in the
    // other process.
    if (s->attached)
    {
        Process *proc = Kernel::instance->getProcessManager()->get(s->pid);
        if (proc)
        {
            ProcessShares & shares = proc->getShares();
            Size size = shares.m_shares.size();

            // Mark all process shares detached in the other process
            for (Size i = 0; i < size; i++)
            {
                MemoryShare *otherShare = (MemoryShare *) shares.m_shares.get(i);
                if (otherShare && otherShare->pid == m_pid
                               && otherShare->coreId == s->coreId)
                {
                    otherShare->attached = false;
                }
            }
        }
    }
    else
    {
        // Only release physical memory pages if the other
        // process already detached earlier
        Allocator *alloc = Kernel::instance->getAllocator();

        for (Size i = 0; i < s->range.size; i += PAGESIZE)
            alloc->release(s->range.phys + i);
    }
    // Unmap the share
    m_memory->unmapRange(&s->range);

    // Remove share from our administration
    m_shares.remove(idx);
    delete s;
    return Success;
}

ProcessShares::Result ProcessShares::readShare(MemoryShare *share)
{
    Size size = m_shares.size();
    const MemoryShare *s = 0;

    for (Size i = 0; i < size; i++)
    {
        if ((s = m_shares.get(i)) != ZERO)
        {
            if (s->pid == share->pid &&
                s->coreId == share->coreId &&
                s->tagId == share->tagId)
            {
                MemoryBlock::copy(share, s, sizeof(MemoryShare));
                return Success;
            }
        }
    }
    return NotFound;
}
