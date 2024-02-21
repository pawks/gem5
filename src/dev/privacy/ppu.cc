/*  Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
    Created On:  Tue Feb 06, 2024 18:24:32 
    Author(s): 
    - Pawan Kumar Sanjaya <pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 * 
 */
#include <random>
#include <limits>
#include <math.h>
#include <vector>
#include <cstdlib>
#include "dev/privacy/ppu.hh"

#include "base/trace.hh"
#include "debug/PPU.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

using namespace std;

namespace gem5{

    vector<string> split(const string& str, const string& delim)
    {
        vector<string> result;
        size_t start = 0;
    
        for (size_t found = str.find(delim); found != string::npos; found = str.find(delim, start))
        {
            result.emplace_back(str.begin() + start, str.begin() + found);
            start = found + delim.size();
        }
        if (start != str.size())
            result.emplace_back(str.begin() + start, str.end());
        return result;      
    } 

    template <typename T>
    auto caster(T value){return reinterpret_cast<std::tuple_element_t<size_t(log2(sizeof(T))), std::tuple<uint8_t,
                                                                                                      uint16_t,
                                                                                                      uint32_t,
                                                                                                      uint64_t>>&>(value);}
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
    PPU::PPU(const Params &p): BasicPioDevice(p,p.num_devices*(num_regs_per_device+1)*8){
        cregs = new double[p.num_devices*num_regs_per_device];
        int j = 0;
        /* for(double i=1.0; j<p.num_devices*num_regs_per_device; i=i*2,++j) */
        /*     cregs[j] = (j==1)? 0 : (double)(i+1); */
        cregs[0] = 1.0;
        cregs[1] = 0;
        cregs[2] = 3.0;
        for(int i=0; i<params().num_devices*num_regs_per_device; ++i)
            DPRINTF(PPU, "read to address reg=%d val=%lf(%#llx)\n", i,cregs[i],caster(cregs[i]));

    }
    Tick PPU::read(PacketPtr pkt){
        for(int i=0; i<params().num_devices*num_regs_per_device; ++i)
            DPRINTF(PPU, "read to address reg=%d val=%lf(%#llx)\n", i,cregs[i],caster(cregs+i));

        pkt->makeAtomicResponse();
        assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
        double elem = 256.0;
                uint64_t addr = pkt->getAddr()-pioAddr;
        DPRINTF(PPU, "read to address va=%#x offset=%#x size=%d\n", pkt->getAddr(),addr, pkt->getSize());
        if (addr < 8*params().num_devices*num_regs_per_device)
            elem = cregs[addr/8];
        else {
            // Budget control and update
            uint64_t device = ((addr/8)-(params().num_devices*num_regs_per_device));
            DPRINTF(PPU,"Data read for device: %#x\n",device);
            if(cregs[device*num_regs_per_device]+cregs[(device*num_regs_per_device)+1] >
                    cregs[(device*num_regs_per_device)+2])
            {
                DPRINTF(PPU, "Noise budget exceeded.\n");
                pkt->setBadAddress();
            }
            else {
                std::string str;
                in >> str;
                std::string delim = ",";
                auto tokens = split(str,delim);
                elem = stod(tokens[3]);
                std::uniform_real_distribution<double> distribution(-2,2);
                cregs[(device*num_regs_per_device)+1] += cregs[(device*num_regs_per_device)];
                double stdev = cregs[(device*num_regs_per_device)]; 
                double noise = distribution(generator);
                DPRINTF(PPU, "Uniform Random Noise: %lf(%#llx).\n",noise, caster(noise));
                noise = -1 * stdev * log(abs(noise)); 
                DPRINTF(PPU, "Raw: %lf(%#x) Noise: %lf Out: %lf(%#x).\n",
                        elem, caster(elem),noise,elem+noise, 
                        caster(elem+noise));
                elem = elem + noise;
            }
        }
        DPRINTF(PPU, "Responding with data %lf(%#llx)\n",elem,caster(elem));
        
        switch (pkt->getSize()) {
            case sizeof(uint64_t):
                pkt->setLE(caster(elem));
                break;
            case sizeof(uint32_t):
                pkt->setLE((uint32_t)caster(elem));
                break;
            case sizeof(uint16_t):
                pkt->setLE((uint16_t)caster(elem));
                break;
            case sizeof(uint8_t):
                pkt->setLE((uint8_t)caster(elem));
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
                    ((uint64_t)(~(1<<pkt->getSize())-1) & (uint64_t)cregs[addr]);
        return pioDelay;
    }

}
