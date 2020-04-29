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
#include <ExecutableFormat.h>
#include "CoreServer.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef INTEL
#include <intel/IntelMP.h>
#endif

const char * CoreServer::kernelPath = "/boot/kernel";

CoreServer::CoreServer()
    : ChannelServer<CoreServer, FileSystemMessage>(this)
#ifdef INTEL
    , m_mp(m_apic)
#endif /* INTEL */
{
    m_numRegions = 0;
    m_kernel = ZERO;
    m_kernelImage = ZERO;
    m_coreInfo = ZERO;

#ifdef INTEL
    m_cores = ZERO;
    m_toMaster = ZERO;
    m_fromMaster = ZERO;
    m_toSlave = ZERO;
    m_fromSlave = ZERO;
#endif

    // Register IPC handlers
    addIPCHandler(ReadFile,  &CoreServer::getCoreCount);

    // Because of waitpid() we must send the reply manually before waitpid().
    addIPCHandler(CreateFile, &CoreServer::createProcess, false);
}

int CoreServer::runCore()
{
    FileSystemMessage msg;

    if (m_info.coreId == 0)
        return run();

    while (true)
    {
        // wait from a message of the master core
        receiveFromMaster(&msg);

        if (m_ipcHandlers->at(msg.action))
        {
            m_sendReply = m_ipcHandlers->at(msg.action)->sendReply;
            (this->*(m_ipcHandlers->at(msg.action))->exec)(&msg);

            if (m_sendReply)
            {
                sendToMaster(&msg);
            }
        }
    }
}

void CoreServer::createProcess(FileSystemMessage *msg)
{
    char cmd[128];
    Memory::Range range;

    if (m_info.coreId == 0)
    {
        range.virt = (Address) msg->buffer;
        VMCtl(msg->from, LookupVirtual, &range);
        msg->buffer = (char *) range.phys;

        range.virt = (Address) msg->path;
        VMCtl(msg->from, LookupVirtual, &range);
        msg->path = (char *) range.phys;

        if (sendToSlave(msg->size, msg) != Success)
        {
            ERROR("failed to write channel on core"<<msg->size);
            msg->result = EBADF;
            return;
        }
        DEBUG("creating program at phys " << (void *) msg->buffer << " on core" << msg->size);

        if (receiveFromSlave(msg->size, msg) != Success)
        {
            ERROR("failed to read channel on core" << msg->size);
            msg->result = EBADF;
            return;
        }
        DEBUG("program created with result " << (int)msg->result << " at core" << msg->size);

        msg->result = ESUCCESS;
        ChannelClient::instance->syncSendTo(msg, msg->from);
    }
    else
    {
        VMCopy(SELF, API::ReadPhys, (Address) cmd, (Address) msg->path, sizeof(cmd));

        range.phys   = (Address) msg->buffer;
        range.virt   = 0;
        range.access = Memory::Readable | Memory::User;
        range.size   = msg->offset;
        VMCtl(SELF, Map, &range);

        pid_t pid = spawn(range.virt, msg->offset, cmd);
        int status;

        // reply to master before calling waitpid()
        msg->result = ESUCCESS;
        sendToMaster(msg);

        // Wait until the spawned process completes
        waitpid(pid, &status, 0);
    }
}

void CoreServer::getCoreCount(FileSystemMessage *msg)
{
    DEBUG("");

    if (m_info.coreId == 0)
    {
#ifdef INTEL
        if (m_cores)
            msg->size = m_cores->getCores().count();
        else
            msg->size = 1;
#else
        msg->size = 1;
#endif
        msg->result = ESUCCESS;
    }
    else
        msg->result = EINVAL;
}

CoreServer::Result CoreServer::test()
{
#ifdef INTEL
    if (m_info.coreId != 0)
    {
        FileSystemMessage msg;
        msg.type   = ChannelMessage::Request;
        msg.action = StatFile;
        msg.path = (char *)0x12345678;
        msg.size = m_info.coreId;

        sendToMaster(&msg);
    }
    else
    {
        FileSystemMessage msg;
        Size numCores = m_cores->getCores().count();

        for (Size i = 1; i < numCores; i++)
        {
            receiveFromSlave(i, &msg);

            if (msg.action == StatFile)
            {
                NOTICE("core" << i << " send a Ping");
            }
            else
            {
                ERROR("invalid message received from core" << i);
            }
        }

    }
#endif /* INTEL */
    return Success;
}

CoreServer::Result CoreServer::initialize()
{
    Result r;

#ifdef INTEL
    // Register IPI vector
    // TODO: minus 32 because of IRQ() in IntelKernel
    ProcessCtl(SELF, WatchIRQ, IPIVector - 32);
#endif /* INTEL */

    // Only core0 needs to start other coreservers
    if (m_info.coreId != 0)
        return setupChannels();

    if ((r = loadKernel()) != Success)
        return r;

    if ((r = discover()) != Success)
        return r;

    if ((r = setupChannels()) != Success)
        return r;

    return bootAll();
}

CoreServer::Result CoreServer::loadKernel()
{
    struct stat st;
    int fd;

    DEBUG("Opening : " << kernelPath);

    // Read the program image
    if (stat(kernelPath, &st) != 0)
        return IOError;

    if ((fd = open(kernelPath, O_RDONLY)) < 0)
        return IOError;

    m_kernelImage = new u8[st.st_size];
    if (read(fd, m_kernelImage, st.st_size) != st.st_size)
    {
        return IOError;
    }
    close(fd);

    // Attempt to read executable format
    if (ExecutableFormat::find(m_kernelImage, st.st_size, &m_kernel) != ExecutableFormat::Success)
        return ExecError;

    // Retrieve memory regions
    m_numRegions = 16;
    if (m_kernel->regions(m_regions, &m_numRegions) != ExecutableFormat::Success)
        return ExecError;

    DEBUG("kernel loaded");
    return Success;
}

CoreServer::Result CoreServer::bootCore(uint coreId, CoreInfo *info,
                                        ExecutableFormat::Region *regions)
{
    SystemInformation sysInfo;
    DEBUG("Reserving: " << (void *)info->memory.phys << " size=" <<
            info->memory.size << " available=" << sysInfo.memoryAvail);

    // Claim the core's memory
    if (VMCtl(SELF, ReserveMem, &info->memory) != API::Success)
    {
        ERROR("failed to reserve memory for core" << coreId <<
              " at " << (void *)info->memory.phys);
        return OutOfMemory;
    }

    DEBUG("Starting core" << coreId << " with "
          << info->memory.size / 1024 / 1024 << "MB");

    // Map the kernel
    for (Size i = 0; i < m_numRegions; i++)
    {
        Memory::Range range;
        range.phys = info->memory.phys + regions[i].virt;
        range.virt = 0;
        range.size = regions[i].size;
        range.access = Memory::Readable | Memory::Writable |
                       Memory::User;

        // Map the target kernel's memory for regions[i].size
        if (VMCtl(SELF, Map, &range) != 0)
        {
            return OutOfMemory;
        }
        // Copy the kernel to the target core's memory
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
        API::Result r = VMCopy(SELF, API::Write, (Address) regions[i].data,
                               range.virt,
                               regions[i].size);
        if ((Size)r != regions[i].size)
            return MemoryError;

        // Unmap the target kernel's memory
        if (VMCtl(SELF, UnMap, &range) != API::Success)
        {
            return MemoryError;
        }

        DEBUG(kernelPath << "[" << i << "] = " << (void *) m_regions[i].virt);
    }

    // Copy the BootImage after the kernel.
    Memory::Range range;
    range.phys = info->bootImageAddress;
    range.virt = 0;
    range.size = info->bootImageSize;
    range.access = Memory::Readable | Memory::Writable | Memory::User;

    // Map BootImage buffer
    if (VMCtl(SELF, Map, &range) != API::Success)
    {
        return OutOfMemory;
    }
    // Copy the BootImage
    Error r = VMCopy(SELF, API::Write, sysInfo.bootImageAddress,
                     range.virt, sysInfo.bootImageSize);
    if (r != (Error) sysInfo.bootImageSize)
        return MemoryError;

    // Unmap the BootImage
    if (VMCtl(SELF, UnMap, &range) != API::Success)
        return MemoryError;

#ifdef INTEL
    // Signal the core to boot
    if (m_mp.boot(info) != IntelMP::Success) {
        ERROR("failed to boot core" << coreId);
        return BootError;
    } else {
        NOTICE("core" << coreId << " started");
    }
#endif
    return Success;
}


CoreServer::Result CoreServer::discover()
{
#ifdef INTEL
    SystemInformation sysInfo;
    Size memPerCore = 0;

    if (m_acpi.initialize() == IntelACPI::Success &&
        m_acpi.discover() == IntelACPI::Success)
    {
        NOTICE("using ACPI as CoreManager");
        m_mp.discover();
        m_cores = &m_acpi;
    }
    else if (m_mp.discover() == IntelMP::Success)
    {
        NOTICE("using MPTable as CoreManager");
        m_cores = &m_mp;
    }
    else
    {
        ERROR("no CoreManager found (ACPI or MPTable)");
        return NotFound;
    }
    List<uint> & cores = m_cores->getCores();
    if (cores.count() == 0)
    {
        ERROR("no cores found");
        return NotFound;
    }

    memPerCore = sysInfo.memorySize / cores.count();
    memPerCore /= MegaByte(4);
    memPerCore *= MegaByte(4);

    NOTICE("found " << cores.count() << " cores -- " <<
            (memPerCore / 1024 / 1024) << "MB per core");

    // Allocate CoreInfo for each core
    m_coreInfo = new Index<CoreInfo>(cores.count());

    // Boot each core
    for (ListIterator<uint> i(cores); i.hasCurrent(); i++)
    {
        uint coreId = i.current();

        if (coreId != 0)
        {
            CoreInfo *info = new CoreInfo;
            m_coreInfo->insert(coreId, *info);
            MemoryBlock::set(info, 0, sizeof(CoreInfo));
            info->coreId = coreId;
            info->memory.phys = memPerCore * coreId;
            info->memory.size = memPerCore - PAGESIZE;
            info->kernel.phys = info->memory.phys;
            info->kernel.size = MegaByte(4);
            info->bootImageAddress = info->kernel.phys + info->kernel.size;
            info->bootImageSize    = sysInfo.bootImageSize;
            info->coreChannelAddress = info->bootImageAddress + info->bootImageSize;
            info->coreChannelAddress += PAGESIZE - (info->bootImageSize % PAGESIZE);
            info->coreChannelSize    = PAGESIZE * 4;
            clearPages(info->coreChannelAddress, info->coreChannelSize);

            m_kernel->entry(&info->kernelEntry);
            info->timerCounter = sysInfo.timerCounter;
            strlcpy(info->kernelCommand, kernelPath, KERNEL_PATHLEN);
        }
    }
#endif
    return Success;
}

CoreServer::Result CoreServer::bootAll()
{
#ifdef INTEL
    List<uint> & cores = m_cores->getCores();
    if (cores.count() == 0)
    {
        ERROR("no cores found");
        return NotFound;
    }

    // Boot each core
    for (ListIterator<uint> i(cores); i.hasCurrent(); i++)
    {
        uint coreId = i.current();

        if (coreId != 0)
        {
            bootCore(coreId, (CoreInfo *)m_coreInfo->get(coreId), m_regions);
        }
    }
#endif
    return Success;
}

CoreServer::Result CoreServer::clearPages(Address addr, Size size)
{
    Memory::Range range;

    range.phys = addr;
    range.virt = ZERO;
    range.size = size;
    range.access = Memory::User | Memory::Readable | Memory::Writable;
    VMCtl(SELF, Map, &range);

    MemoryBlock::set((void *) range.virt, 0, size);

    VMCtl(SELF, UnMap, &range);
    return Success;
}

CoreServer::Result CoreServer::setupChannels()
{
#ifdef INTEL
    SystemInformation info;

    if (info.coreId == 0)
    {
        Size numCores = m_cores->getCores().count();

        m_toSlave    = new Index<MemoryChannel>(numCores);
        m_fromSlave  = new Index<MemoryChannel>(numCores);

        for (Size i = 1; i < numCores; i++)
        {
            MemoryChannel *ch = new MemoryChannel();
            CoreInfo *coreInfo = (CoreInfo *) m_coreInfo->get(i);
            ch->setMode(Channel::Producer);
            ch->setMessageSize(sizeof(FileSystemMessage));
            ch->setPhysical(coreInfo->coreChannelAddress + (PAGESIZE * 2),
                            coreInfo->coreChannelAddress + (PAGESIZE * 3));
            m_toSlave->insert(i, *ch);

            ch = new MemoryChannel();
            ch->setMode(Channel::Consumer);
            ch->setMessageSize(sizeof(FileSystemMessage));
            ch->setPhysical(coreInfo->coreChannelAddress,
                            coreInfo->coreChannelAddress + PAGESIZE);
            m_fromSlave->insert(i, *ch);
        }
    }
    else
    {
        m_toMaster = new MemoryChannel();
        m_toMaster->setMode(Channel::Producer);
        m_toMaster->setMessageSize(sizeof(FileSystemMessage));
        m_toMaster->setPhysical(info.coreChannelAddress,
                                info.coreChannelAddress + PAGESIZE);

        m_fromMaster = new MemoryChannel();
        m_fromMaster->setMode(Channel::Consumer);
        m_fromMaster->setMessageSize(sizeof(FileSystemMessage));
        m_fromMaster->setPhysical(info.coreChannelAddress + (PAGESIZE * 2),
                                  info.coreChannelAddress + (PAGESIZE * 3));
    }
#endif /* INTEL */
    return Success;
}

CoreServer::Result CoreServer::receiveFromMaster(FileSystemMessage *msg)
{
    Channel::Result result = Channel::NotFound;

    // wait from a message of the master core
    while (result != Channel::Success)
    {
        for (uint i = 0; i < MaxMessageRetry && result != Channel::Success; i++)
        {
            result = m_fromMaster->read(msg);
        }

        // Wait for IPI which will wake us
        ProcessCtl(SELF, EnableIRQ, IPIVector);
        ProcessCtl(SELF, EnterSleep, 0, 0);
    }

    return Success;
}

CoreServer::Result CoreServer::sendToMaster(FileSystemMessage *msg)
{
    while (m_toMaster->write(msg) != Channel::Success)
        ;

    return Success;
}

CoreServer::Result CoreServer::receiveFromSlave(uint coreId, FileSystemMessage *msg)
{
    MemoryChannel *ch = (MemoryChannel *) m_fromSlave->get(coreId);
    if (!ch)
        return IOError;

    while (ch->read(msg) != Channel::Success)
        ;

    return Success;
}

CoreServer::Result CoreServer::sendToSlave(uint coreId, FileSystemMessage *msg)
{
    MemoryChannel *ch = (MemoryChannel *) m_toSlave->get(coreId);
    if (!ch)
    {
        ERROR("cannot retrieve MemoryChannel for core" << coreId);
        msg->result = ENOENT;
        return IOError;
    }

    if (ch->write(msg) != Channel::Success)
    {
        ERROR("failed to write channel on core" << coreId);
        msg->result = EBADF;
        return IOError;
    }

#ifdef INTEL
    // Send IPI to ensure the slave wakes up for the message
    if (m_apic.sendIPI(coreId, IPIVector) != IntController::Success)
    {
        ERROR("failed to send IPI to core" << coreId);
        return IOError;
    }

#endif /* INTEL */
    return Success;
}
