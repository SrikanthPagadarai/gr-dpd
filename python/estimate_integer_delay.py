# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Estimate Integer Delay
# Generated: Thu Aug 17 17:14:29 2017
##################################################

import os
import sys

from gnuradio import analog
from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import dpd


class estimate_integer_delay(gr.hier_block2):

    def __init__(self, NFFT=2048, STS_len=0, STS_signed_ampl_diff=[-1,-1,-1,-1,1,1,1,1,1,-1], corr_len=0, ovx=6, peak_val=0):
        gr.hier_block2.__init__(
            self, "Estimate Integer Delay",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.NFFT = NFFT
        self.STS_len = STS_len
        self.STS_signed_ampl_diff = STS_signed_ampl_diff
        self.corr_len = corr_len
        self.ovx = ovx
        self.peak_val = peak_val

        ##################################################
        # Blocks
        ##################################################
        self.dpd_ampl_diff_0 = dpd.ampl_diff()
        self.fir_filter_xxx_0 = filter.fir_filter_fff(1, (STS_signed_ampl_diff[0:corr_len+1]))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.dpd_peak_detect_0 = dpd.peak_detect(NFFT, ovx, peak_val)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_float_to_complex_0, 0), (self.dpd_peak_detect_0, 0))
        self.connect((self.dpd_peak_detect_0, 0), (self, 0))
        self.connect((self.fir_filter_xxx_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.dpd_ampl_diff_0, 0), (self.fir_filter_xxx_0, 0))
        self.connect((self, 0), (self.dpd_peak_detect_0, 1))
        self.connect((self, 0), (self.dpd_ampl_diff_0, 0))

    def get_NFFT(self):
        return self.NFFT

    def set_NFFT(self, NFFT):
        self.NFFT = NFFT

    def get_STS_len(self):
        return self.STS_len

    def set_STS_len(self, STS_len):
        self.STS_len = STS_len

    def get_STS_signed_ampl_diff(self):
        return self.STS_signed_ampl_diff

    def set_STS_signed_ampl_diff(self, STS_signed_ampl_diff):
        self.STS_signed_ampl_diff = STS_signed_ampl_diff
        self.fir_filter_xxx_0.set_taps((self.STS_signed_ampl_diff[0:self.corr_len+1]))

    def get_corr_len(self):
        return self.corr_len

    def set_corr_len(self, corr_len):
        self.corr_len = corr_len
        self.fir_filter_xxx_0.set_taps((self.STS_signed_ampl_diff[0:self.corr_len+1]))

    def get_ovx(self):
        return self.ovx

    def set_ovx(self, ovx):
        self.ovx = ovx

    def get_peak_val(self):
        return self.peak_val

    def set_peak_val(self, peak_val):
        self.peak_val = peak_val
