/*  Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
 *  Created On:  Tue Feb 06, 2024 18:33:29 
 *  Author(s): 
 *  - Pawan Kumar Sanjaya <pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 */
#ifndef __PPU__
#define __PPU__

namespace gem5{
    class PPU: public BasicPioDevice
    {
        protected:
          uint8_t retData8;
          uint16_t retData16;
          uint32_t retData32;
          uint64_t retData64;
      
        public:
          PARAMS(PPU);
          PPU(const Params &p);
          virtual Tick read(PacketPtr pkt);
          virtual Tick write(PacketPtr pkt);
      };
           
    }
}
#endif

