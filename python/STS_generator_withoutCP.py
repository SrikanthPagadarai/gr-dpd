# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: STS without CP
# Generated: Thu Aug 17 17:16:05 2017
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import fft
from gnuradio import gr
from gnuradio.fft import window
from gnuradio.filter import firdes
import dpd


class STS_generator_withoutCP(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "STS without CP",
            gr.io_signature(0, 0, 0),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*2048),
        )

        ##################################################
        # Blocks
        ##################################################
        self.fft_vxx_0 = fft.fft_vcc(2048, False, (window.rectangular(2048)), False, 1)
        self.dpd_stream_to_zero_padded_vector_0 = dpd.stream_to_zero_padded_vector(1680, 2048)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_float*1, 840)
        self.blocks_vector_source_x_0 = blocks.vector_source_f((0,0,0), True, 1, [])
        self.blocks_vector_insert_x_0 = blocks.vector_insert_f((0, 0, 0), 4, 0)
        self.blocks_stream_to_vector_0_0 = blocks.stream_to_vector(gr.sizeof_float*1, 840)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_float*1, 840)
        self.blocks_stream_mux_0_0 = blocks.stream_mux(gr.sizeof_float*840, (1, 1))
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_float*1, (1, 3))
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vff((2, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((2, ))
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 0)
        self.STS_488_0 = blocks.vector_source_f((-1,-1,1,-1,-1,-1,-1,-1,-1,-1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,1,1,1,1,1,1,1,1,1,-1,-1,1,-1,1,-1,1,-1,1,-1,1,1,1,-1,-1,1,1,-1,-1,1,1,-1,1,-1,-1,-1,1,-1,-1,-1,1,-1,-1,1,1,1,1,-1,-1,-1,-1,1,1,1,-1,1,-1,1,1,1,1,1,-1,1,-1,-1,1,1,-1,1,-1,1,-1,-1,1,1,1,-1,1,1,-1,-1,1,1,1,-1,1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,1,1,1,-1,-1,1,-1,-1,-1,-1,1,-1,1,-1,-1,-1,1,1,1,1,1,-1,-1,1,1,1,1,-1,1,-1,1,-1,-1,-1,1,-1,1,-1,-1,1,1,-1,-1,-1,-1,1,1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,1,1,1,1,1,-1,-1,-1,-1,-1,1,-1,1,-1,1,-1,-1,-1,-1,-1,-1,1,1,-1,-1), False, 1, [])
        self.STS_277 = blocks.vector_source_f((1,1,-1,-1,-1,1,-1,1,-1,1,1,-1,1,1,1,1,-1,-1,1,1,-1,1,1,-1,1,-1,1,1,1,-1,1,1,-1,1,1,-1,-1,1,-1,1,1,-1,1,1,-1,1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,1,1,1,-1,-1,-1,1,1,1,-1,-1,1,-1,1,1,1,1,-1,1,-1,-1,-1,1,1,-1,1,-1,1,1,-1,-1,-1,-1,1,-1,-1,1,1,-1,1,1,1,1,1,-1,-1,-1,1,-1,-1,1,-1,1,-1,1,1,1,1,-1,-1,-1,1,1,-1,-1,1,-1,1,-1,-1,-1,-1,1,-1,-1,-1,1,1,-1,-1,-1,-1,-1,1,1,1,1,-1,1,1,1,1,1,1,-1,1,-1,1,1,-1,1,-1,1,-1,1,1,-1,-1,1,-1,-1,1,1,-1,-1,1,-1,-1,-1,1,1,1,-1,1,1,1,-1,-1,-1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,-1,1,1,-1,1,1,-1,-1,-1,-1,-1,-1), False, 1, [])

        ##################################################
        # Connections
        ##################################################
        self.connect((self.STS_277, 0), (self.blocks_stream_mux_0, 0))
        self.connect((self.STS_488_0, 0), (self.blocks_vector_insert_x_0, 0))
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_float_to_complex_0, 0), (self.dpd_stream_to_zero_padded_vector_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_stream_to_vector_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_stream_to_vector_0_0, 0))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.blocks_stream_mux_0_0, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.blocks_stream_mux_0_0, 0))
        self.connect((self.blocks_stream_to_vector_0_0, 0), (self.blocks_stream_mux_0_0, 1))
        self.connect((self.blocks_vector_insert_x_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_mux_0, 1))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.dpd_stream_to_zero_padded_vector_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.fft_vxx_0, 0), (self, 0))
