/*  Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
    Created On:  Tue Feb 06, 2024 18:24:32 
    Author(s): 
    - Pawan Kumar Sanjaya <pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 * 
 */
#include <random>
#include <limits>
#include <math.h>
#include <cstdlib>
#include "dev/privacy/ppu.hh"

#include "base/trace.hh"
#include "debug/PPU.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"



namespace gem5{
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
    PPU::PPU(const Params &p): BasicPioDevice(p,p.num_devices*(num_regs_per_device+1)){
        cregs = new uint64_t[p.num_devices*num_regs_per_device];
        for(int i=0; i<p.num_devices*num_regs_per_device; ++i)
            cregs[i] = 0;
    }
    Tick PPU::read(PacketPtr pkt){
        pkt->makeAtomicResponse();
        DPRINTF(PPU, "read to address va=%#x size=%d\n", pkt->getAddr(), pkt->getSize());
        assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
        uint64_t elem = 256;
        auto addr = pkt->getAddr()-pioAddr;
        if (addr <= params().num_devices*num_regs_per_device)
            elem = cregs[addr];
        else {
            // Budget control and update
            auto device = (addr-(params().num_devices*num_regs_per_device)) / sizeof(uint64_t);
            if(cregs[device*num_regs_per_device]+cregs[(device*num_regs_per_device)+1] <=
                    cregs[(device*num_regs_per_device)+1])
                pkt->setBadAddress();
            else {
                std::uniform_real_distribution<double> distribution(min_value,max_value);
                cregs[(device*num_regs_per_device)+1] += cregs[(device*num_regs_per_device)];
                double stdev = cregs[(device*num_regs_per_device)]; 
                double noise = distribution(generator);
                noise = -1 * (stdev * sgn(noise-0.5)) * log(1-2*abs(noise-0.5)); 
                elem = uint64_t((double)elem + noise);
            }
        }
        
        switch (pkt->getSize()) {
            case sizeof(uint64_t):
                pkt->setLE((uint64_t)elem);
                break;
            case sizeof(uint32_t):
                pkt->setLE((uint32_t)elem);
                break;
            case sizeof(uint16_t):
                pkt->setLE((uint16_t)elem);
                break;
            case sizeof(uint8_t):
                pkt->setLE((uint8_t)elem);
                break;
            default:
                panic("invalid access size!");

        }
        return pioDelay;
    }
    Tick PPU::write(PacketPtr pkt){
        pkt->makeAtomicResponse();
        DPRINTF(PPU, "write to address va=%#x size=%d val=%#x\n",pkt->getAddr(), pkt->getSize(),
                pkt->getLE<uint64_t>());
        assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
        auto addr=pkt->getAddr()-pioAddr;
        if (addr <= params().num_devices*num_regs_per_device)
            cregs[addr] = (((1<<pkt->getSize())-1) & pkt->getLE<uint64_t>()) | 
                    ((~(1<<pkt->getSize())-1) & cregs[addr]);
        return pioDelay;
    }

}
