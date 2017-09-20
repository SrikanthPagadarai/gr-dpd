#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat Jun 24 16:38:45 2017
##################################################

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from gnuradio import digital
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.fft import logpwrfft
from gnuradio.filter import firdes
from optparse import OptionParser
import dpd
import sys, time, os, getopt
from hier_dpd_elliptic_filter import hier_dpd_elliptic_filter
def struct(data): return type('Struct', (object,), data)()

class top_block(gr.top_block):

    def __init__(self, center_freq, sbx_db_tx_gain, sbx_db_rx_gain, bb_ampl, dir_filename1, dir_filename2):
        gr.top_block.__init__(self)

        ##################################################
        # Variables
        ##################################################
        self.TxParams = TxParams = struct({'NFFT': 2048, 'UsedSC': 1680, 'CPlen': 512, 'Ovx': 6, 'N': 5000, 'SampRate': 100e6/50.0, 'CenterFreq': center_freq})
        self.lo_offset = lo_offset = 3.0*TxParams.SampRate

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(TxParams.SampRate)
        self.uhd_usrp_source_0.set_center_freq(TxParams.CenterFreq, 0)
        self.uhd_usrp_source_0.set_gain(sbx_db_rx_gain, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(TxParams.SampRate)
        self.uhd_usrp_sink_0.set_center_freq(uhd.tune_request(TxParams.CenterFreq, rf_freq=(TxParams.CenterFreq+lo_offset),rf_freq_policy=uhd.tune_request.POLICY_MANUAL), 0)
        self.uhd_usrp_sink_0.set_gain(sbx_db_tx_gain, 0)
        self.logpwrfft_x_0 = logpwrfft.logpwrfft_c(
        	sample_rate=TxParams.SampRate,
        	fft_size=8192,
        	ref_scale=2,
        	frame_rate=30,
        	avg_alpha=1.0,
        	average=False,
        )
        self.dpd_Agilent_N1996A_0 = dpd.Agilent_N1996A('169.254.251.20', TxParams.CenterFreq, TxParams.SampRate, 5.1e3, 1001)
        self.blocks_file_sink_1 = blocks.file_sink(gr.sizeof_float*8192, dir_filename2, False)
        self.blocks_file_sink_1.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*4096, dir_filename1, False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.hier_dpd_elliptic_filter_0 = hier_dpd_elliptic_filter()
        self.dpd_upsample_0 = dpd.upsample(
            ovx=TxParams.Ovx,
        )
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(TxParams.NFFT, (TxParams.NFFT+TxParams.CPlen), 32, '')
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vcc((bb_ampl, ))
        self.basic_qpskofdm_tx_withoutCP_0 = dpd.basic_qpskofdm_tx_withoutCP(
            N=TxParams.N,
            NFFT=TxParams.NFFT,
            used_sc=TxParams.UsedSC,
            syms_filename='/tmp/qpsk_syms.bin'
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.basic_qpskofdm_tx_withoutCP_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.dpd_upsample_0, 0))
        self.connect((self.dpd_upsample_0, 0), (self.hier_dpd_elliptic_filter_0, 0))
        self.connect((self.hier_dpd_elliptic_filter_0, 0), (self.blocks_multiply_const_vxx_1, 0))
        self.connect((self.logpwrfft_x_0, 0), (self.blocks_file_sink_1, 0))
        self.connect((self.dpd_Agilent_N1996A_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.logpwrfft_x_0, 0))

    def set_sbx_db_tx_gain(self, sbx_db_tx_gain):
        self.sbx_db_tx_gain = sbx_db_tx_gain
        self.uhd_usrp_source_0.set_gain(self.sbx_db_tx_gain, 0)

    def set_sbx_db_rx_gain(self, sbx_db_rx_gain):
        self.sbx_db_rx_gain = sbx_db_rx_gain
        self.uhd_usrp_sink_0.set_gain(self.sbx_db_rx_gain, 0)

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)        

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.uhd_usrp_source_0.set_center_freq(self.center_freq, 0)        
        self.uhd_usrp_sink_0.set_center_freq(self.center_freq, 0)              

def main(argv):

    try:
       opts, args = getopt.getopt(argv,"hf:c:g:a:",["center_freq=", "sbx_db_tx_gain=", "sbx_db_rx_gain=", "bb_ampl="])
    except getopt.GetoptError:
       print 'ofdm_TXpower_measurement_N210SBX.py -f <center_freq> -c <sbx_db_tx_gain> -g <sbx_db_rx_gain> -a <bb_ampl>'
       sys.exit(2)

    for opt, arg in opts:
       if opt == '-h':
          print 'ofdm_TXpower_measurement_N210SBX.py -f <center_freq> -c <sbx_db_tx_gain> -g <sbx_db_rx_gain> -a <bb_ampl>'
          sys.exit()
       elif opt in ("-f", "--center_freq"):
          center_freq = float(arg)
          print center_freq
       elif opt in ("-c", "--sbx_db_tx_gain"):
          sbx_db_tx_gain = float(arg)
          print sbx_db_tx_gain
       elif opt in ("-g", "--sbx_db_rx_gain"):
          sbx_db_rx_gain = float(arg)
          print sbx_db_rx_gain
       elif opt in ("-a", "--bb_ampl"):
          bb_ampl = float(arg)
          print bb_ampl

    if center_freq < 400e6 or center_freq > 4400e6:
       print 'Error! Invalid choice of center frequency for SBX.'
       sys.exit()
    
    if sbx_db_tx_gain < 0.0 or sbx_db_tx_gain > 31.5:
       print 'Error! Invalid choice of Tx gain for SBX.'
       sys.exit()

    if sbx_db_rx_gain < 0.0 or sbx_db_rx_gain > 31.5:
       print 'Error! Invalid choice of Rx gain for SBX.'
       sys.exit()

    dir_name = "./ofdm_TXpower_measurements_N210SBX"
    if not os.path.exists(dir_name):
       os.makedirs(dir_name)

    filename1 = "ofdm_RXpowerSA_N210SBX_f" + str(int(center_freq)) + "_c" + str(int(sbx_db_tx_gain*10)) + "_g" + str(int(sbx_db_rx_gain*10)) + "_a" + str(int(bb_ampl*100)) + ".bin"
    dir_filename1 = dir_name + "/" + filename1

    filename2 = "ofdm_RXpowerGRC_N210SBX_f" + str(int(center_freq)) + "_c" + str(int(sbx_db_tx_gain*10)) + "_g" + str(int(sbx_db_rx_gain*10)) + "_a" + str(int(bb_ampl*100)) + ".bin"
    dir_filename2 = dir_name + "/" + filename2    

    tb = top_block(center_freq, sbx_db_tx_gain, sbx_db_rx_gain, bb_ampl, dir_filename1, dir_filename2)    
    tb.start()    
    time.sleep(20)

    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main(sys.argv[1:])
