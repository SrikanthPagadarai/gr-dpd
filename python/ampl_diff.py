# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Amplitude Difference
# Author: Srikanth Pagadarai
##################################################

from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes
import dpd


class ampl_diff(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "Amplitude Difference ",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_float*1),
        )

        ##################################################
        # Blocks
        ##################################################
        self.dpd_signum_func_0 = dpd.signum_func()
        self.blocks_sub_xx_0 = blocks.sub_ff(1)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 1)
        self.blocks_complex_to_mag_0_0 = blocks.complex_to_mag(1)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_sub_xx_0, 0))
        self.connect((self.blocks_complex_to_mag_0_0, 0), (self.blocks_sub_xx_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.blocks_complex_to_mag_0_0, 0))
        self.connect((self.blocks_sub_xx_0, 0), (self.dpd_signum_func_0, 0))
        self.connect((self.dpd_signum_func_0, 0), (self, 0))
        self.connect((self, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self, 0), (self.blocks_delay_0, 0))
