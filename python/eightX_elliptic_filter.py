# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: gr-dpd Elliptic Filter
# Author: Srikanth Pagadarai
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes


class eightX_elliptic_filter(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "8X Elliptic Filter",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Blocks
        ##################################################
        self.iir_filter_xxx0 = filter.iir_filter_ccf(([2.23383305893211e-05, 2.23383305893211e-05, 0.0]), ([1.0, -0.922725273048746, 0.0]), False)
        self.iir_filter_xxx1 = filter.iir_filter_ccf(([1.0, -0.980105379089171, 1.00000000000066]), ([1.0, -1.84581599510221, 0.868593530229019]), False)
        self.iir_filter_xxx2 = filter.iir_filter_ccf(([1.0, -1.61494254279358, 0.999999999688370]), ([1.0, -1.84674060599679, 0.906717861662057]), False)
        self.iir_filter_xxx3 = filter.iir_filter_ccf(([1.0, -1.75922607521818, 1.00000000889469]), ([1.0, -1.84786892387141, 0.943657702106551]), False)
        self.iir_filter_xxx4 = filter.iir_filter_ccf(([1.0, -1.80840888707879, 0.999999938693805]), ([1.0, -1.84925968406881, 0.969701444522271]), False)
        self.iir_filter_xxx5 = filter.iir_filter_ccf(([1.0, -1.82798750700756, 1.00000014346635]), ([1.0, -1.85153083688195, 0.985700459927933]), False)
        self.iir_filter_xxx6 = filter.iir_filter_ccf(([1.0, -1.83519937923591, 0.999999909256144]), ([1.0, -1.85541971981032, 0.995822536479486]), False)        

        ##################################################
        # Connections
        ##################################################
        self.connect((self, 0), (self.iir_filter_xxx0, 0))
        self.connect((self.iir_filter_xxx0, 0), (self.iir_filter_xxx1, 0))
        self.connect((self.iir_filter_xxx1, 0), (self.iir_filter_xxx2, 0))
        self.connect((self.iir_filter_xxx2, 0), (self.iir_filter_xxx3, 0))
        self.connect((self.iir_filter_xxx3, 0), (self.iir_filter_xxx4, 0))
        self.connect((self.iir_filter_xxx4, 0), (self.iir_filter_xxx5, 0))
        self.connect((self.iir_filter_xxx5, 0), (self.iir_filter_xxx6, 0))        
        self.connect((self.iir_filter_xxx6, 0), (self, 0))        
