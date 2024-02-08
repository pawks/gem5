#    Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
#    Created On:  Wed Feb 07, 2024 15:41:32
#    Author(s):
#    - Pawan Kumar Sanjaya<pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks

from m5.objects.ClockedObject import ClockedObject
from m5.params import *
from m5.proxy import *
from m5.util.fdthelper import *
from m5.objects.Device import (
        BasicPioDevice,
)

class PPU(BasicPioDevice):
    type = "PPU"
    cxx_header = "dev/privacy/ppu.hh"
    cxx_class = "gem5::PPU"
    num_devices = Param.UInt64(1,"Number of devices connected to the PPU.")

