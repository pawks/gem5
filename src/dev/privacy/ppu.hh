/*  Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
 *  Created On:  Tue Feb 06, 2024 18:33:29 
 *  Author(s): 
 *  - Pawan Kumar Sanjaya <pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 */
#ifndef __PPU__
#define __PPU__

#include <random>
#include <string>

#include "dev/io_device.hh"
#include "mem/packet.hh"
#include "params/PPU.hh"

#define num_regs_per_device 3
constexpr double max_value {std::numeric_limits<double>::max()}; 
constexpr double min_value {std::numeric_limits<double>::min()}; 

namespace gem5{
    class PPU: public BasicPioDevice
    {
        protected:
            double* cregs;
            std::ifstream in{"/scratch1/ldp/Human-Activity-Recognition-Using-Accelerometer-Data-and-CNN/WISDM_ar_v1.1/WISDM_ar_v1.1_raw.txt"};
            std::default_random_engine generator;
      
        public:
            PARAMS(PPU);
            PPU(const Params &p);
            virtual Tick read(PacketPtr pkt);
            virtual Tick write(PacketPtr pkt);
      };
    }
#endif

