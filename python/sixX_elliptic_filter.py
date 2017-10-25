# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: gr-dpd Elliptic Filter
# Generated: Sun Sep  3 17:05:17 2017
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes


class sixX_elliptic_filter(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "6X Elliptic Filter",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Blocks
        ##################################################
        self.iir_filter_xxx0 = filter.iir_filter_ccf(([3.76858006213517e-05, 2.93811710614945e-05, 3.76858006213517e-05]), ([1.0, -1.79522876160257, 0.813229971141868]), False)
        self.iir_filter_xxx1 = filter.iir_filter_ccf(([1.0, -1.08704401531644, 0.999999999996507]), ([1.0, -1.78339960365674, 0.852957700216366]), False)
        self.iir_filter_xxx2 = filter.iir_filter_ccf(([1.0, -1.51152106622327, 1.00000000036790]), ([1.0, -1.76822683354505, 0.904405728315075]), False)
        self.iir_filter_xxx3 = filter.iir_filter_ccf(([1.0, -1.64313258865777, 0.999999992638329]), ([1.0, -1.75642186862071, 0.945374195176692]), False)
        self.iir_filter_xxx4 = filter.iir_filter_ccf(([1.0, -1.69432470498216, 1.00000004497921]), ([1.0, -1.74970705512270, 0.971597331550094]), False)
        self.iir_filter_xxx5 = filter.iir_filter_ccf(([1.0, -1.71594192641499, 0.999999899667721]), ([1.0, -1.74736589887850, 0.986705438352939]), False)
        self.iir_filter_xxx6 = filter.iir_filter_ccf(([1.0, -1.72411137431316, 1.00000006235034]), ([1.0, -1.74896618676128, 0.996197116129337]), False)   

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
