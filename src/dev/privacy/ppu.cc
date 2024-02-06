/*  Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
    Created On:  Tue Feb 06, 2024 18:24:32 
    Author(s): 
    - Pawan Kumar Sanjaya <pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 * 
 */
#include "dev/privacy/ppu.hh"

#include "base/trace.hh"
#include "debug/Ppu.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

namespace gem5{
    PPU::PPU(const Params &p): BasicPioDevice(p,p.num_devices*16){
    }

}
