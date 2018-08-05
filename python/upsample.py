# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Upsample
# Author: Srikanth Pagadarai
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes


class upsample(gr.hier_block2):

    def __init__(self, ovx=0):
        gr.hier_block2.__init__(
            self, "Upsample",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.ovx = ovx

        ##################################################
        # Blocks
        ##################################################
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_gr_complex*1, (1,(ovx-1)))
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_stream_mux_0, 1))
        self.connect((self.blocks_stream_mux_0, 0), (self, 0))
        self.connect((self, 0), (self.blocks_stream_mux_0, 0))

    def get_ovx(self):
        return self.ovx

    def set_ovx(self, ovx):
        self.ovx = ovx
