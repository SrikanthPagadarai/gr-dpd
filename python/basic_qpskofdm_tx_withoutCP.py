# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Basic QPSK-OFDM Tx
# Author: Srikanth Pagadarai
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import fft
from gnuradio import gr
from gnuradio.fft import window
from gnuradio.filter import firdes
import dpd
import numpy


class basic_qpskofdm_tx_withoutCP(gr.hier_block2):

    def __init__(self, N=10, NFFT=2048, used_sc=1680, syms_filename="/tmp/qpsk_syms.bin"):
        gr.hier_block2.__init__(
            self, "Basic QPSK-OFDM Tx",
            gr.io_signature(0, 0, 0),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*NFFT),
        )

        ##################################################
        # Parameters
        ##################################################
        self.N = N
        self.NFFT = NFFT
        self.used_sc = used_sc        
        self.syms_filename = syms_filename

        ##################################################
        # Blocks
        ##################################################
        self.fft_vxx_0 = fft.fft_vcc(NFFT, False, (window.rectangular(NFFT)), False, 1)
        self.dpd_stream_to_zero_padded_vector_0 = dpd.stream_to_zero_padded_vector(self.used_sc, self.NFFT)
        self.digital_chunks_to_symbols_xx_0 = digital.chunks_to_symbols_ic(((1+1j, 1-1j, -1-1j, -1+1j)), 1)
        self.analog_random_source_x_0 = blocks.vector_source_i(map(int, numpy.random.randint(0, 4, used_sc*N)), False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, syms_filename, False)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc(([1.0/self.NFFT]*self.NFFT))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_random_source_x_0, 0), (self.digital_chunks_to_symbols_xx_0, 0))
        self.connect((self.digital_chunks_to_symbols_xx_0, 0), (self.dpd_stream_to_zero_padded_vector_0, 0))
        self.connect((self.digital_chunks_to_symbols_xx_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.dpd_stream_to_zero_padded_vector_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self, 0))

    def get_N(self):
        return self.N

    def set_N(self, N):
        self.N = N

    def get_NFFT(self):
        return self.NFFT

    def set_NFFT(self, NFFT):
        self.NFFT = NFFT

    def get_used_sc(self):
        return self.used_sc

    def set_used_sc(self, used_sc):
        self.used_sc = used_sc

    def get_syms_filename(self):
        return self.syms_filename

    def set_syms_filename(self, syms_filename):
        self.syms_filename = syms_filename
