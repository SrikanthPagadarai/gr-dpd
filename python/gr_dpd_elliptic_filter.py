# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: gr-dpd Elliptic Filter
# Generated: Sun Sep  3 17:05:17 2017
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes


class gr_dpd_elliptic_filter(gr.hier_block2):

    def __init__(self):
        gr.hier_block2.__init__(
            self, "gr-dpd Elliptic Filter",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Blocks
        ##################################################
        self.iir_filter_xxx_0_2 = filter.iir_filter_ccf(([1.0, -1.652839793269936, 1.000000001400776]), ([1.0,  -1.765877192263684, 0.957051525607135]), False)
        self.iir_filter_xxx_0_1 = filter.iir_filter_ccf(([0.000037246968898, 0.000027087888496, 0.000037246968898]), ([1.0, -1.828895031927531, 0.844175165599898]), False)
        self.iir_filter_xxx_0_0_0_0_0 = filter.iir_filter_ccf(([1.0, -1.727895993039517, 0.999999997157450]), ([1.0,  -1.750253562564659, 0.997194070069812]), False)
        self.iir_filter_xxx_0_0_0_0 = filter.iir_filter_ccf(([1.0, -1.720437070990502, 1.000000006202047]), ([1.0, -1.750131544734019, 0.989897082624026]), False)
        self.iir_filter_xxx_0_0_0 = filter.iir_filter_ccf(([1.0, -1.700521872577379, 0.999999995364357]), ([1.0, -1.755057038241670, 0.978174259992110]), False)
        self.iir_filter_xxx_0_0 = filter.iir_filter_ccf(([1.0, -1.528909466732580 , 0.999999999874678]), ([1.0, -1.784686783288179, 0.923035918015168]), False)
        self.iir_filter_xxx_0 = filter.iir_filter_ccf(([1.0,  -1.123813478589870, 1.000000000000690]), ([1.0,  -1.809313166389885, 0.879004439837204]), False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.iir_filter_xxx_0, 0), (self.iir_filter_xxx_0_0, 0))
        self.connect((self.iir_filter_xxx_0_0, 0), (self.iir_filter_xxx_0_2, 0))
        self.connect((self.iir_filter_xxx_0_0_0, 0), (self.iir_filter_xxx_0_0_0_0, 0))
        self.connect((self.iir_filter_xxx_0_0_0_0, 0), (self.iir_filter_xxx_0_0_0_0_0, 0))
        self.connect((self.iir_filter_xxx_0_0_0_0_0, 0), (self, 0))
        self.connect((self.iir_filter_xxx_0_1, 0), (self.iir_filter_xxx_0, 0))
        self.connect((self.iir_filter_xxx_0_2, 0), (self.iir_filter_xxx_0_0_0, 0))
        self.connect((self, 0), (self.iir_filter_xxx_0_1, 0))
