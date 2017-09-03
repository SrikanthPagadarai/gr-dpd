# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Wiener Power Amplifier
# Generated: Sun Sep  3 14:47:31 2017
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes


class pa_wiener_model(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "Wiener Power Amplifier",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Blocks
        ##################################################
        self.iir_filter_xxx_0 = filter.iir_filter_ccf(((1.0, 0.0, 0.3)), ((1.0, -0.2)), False)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_const_vxx_0_0_0 = blocks.multiply_const_vcc((21.3936+0.4305j, ))
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vcc((-23.0954+4.9680j, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((14.974+0.0519j, ))
        self.blocks_float_to_complex_0_0 = blocks.float_to_complex(1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_complex_to_mag_squared_0_0 = blocks.complex_to_mag_squared(1)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_const_source_x_0_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 0)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.analog_const_source_x_0_0, 0), (self.blocks_float_to_complex_0_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self, 0))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_complex_to_mag_squared_0_0, 0), (self.blocks_float_to_complex_0_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.blocks_float_to_complex_0_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0_0_0, 0), (self.blocks_add_xx_0, 2))
        self.connect((self.blocks_multiply_xx_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.blocks_multiply_const_vxx_0_0_0, 0))
        self.connect((self.iir_filter_xxx_0, 0), (self.blocks_complex_to_mag_squared_0, 0))
        self.connect((self.iir_filter_xxx_0, 0), (self.blocks_complex_to_mag_squared_0_0, 0))
        self.connect((self.iir_filter_xxx_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.iir_filter_xxx_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self, 0), (self.iir_filter_xxx_0, 0))
