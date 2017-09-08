#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat Jun 24 16:38:45 2017
##################################################


from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import dpd
import sys, time, os, getopt


class top_block(gr.top_block):

    def __init__(self, center_freq, b200mini_gain, bb_ampl, dir_filename):
        gr.top_block.__init__(self)

        ##################################################
        # Variables
        ##################################################
        self.tone_freq1 = tone_freq1 = 50e3
        self.tone_freq2 = tone_freq2 = 75e3
        self.samp_rate = samp_rate = 100e6/500
        self.center_freq = center_freq
        self.b200mini_gain = b200mini_gain
        self.bb_ampl = bb_ampl
        self.dir_filename = dir_filename

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_sink_0 = uhd.usrp_sink("type=b200", uhd.io_type_t.COMPLEX_FLOAT32, 1)
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(center_freq, 0)
        self.uhd_usrp_sink_0.set_gain(b200mini_gain, 0)
        self.dpd_Agilent_N1996A_0 = dpd.Agilent_N1996A('169.254.251.20', center_freq, 1000e3, 5.1e3, 1001)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*4096, dir_filename, False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, tone_freq1, bb_ampl, 0)
        self.analog_sig_source_x_1 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, tone_freq2, bb_ampl, 0)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        
        ##################################################
        # Connections
        ##################################################
        self.connect((self.dpd_Agilent_N1996A_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.analog_sig_source_x_1, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.uhd_usrp_sink_0, 0))
        

    def set_tone_freq0(self, tone_freq0):
        self.tone_freq0 = tone_freq0
        self.analog_sig_source_x_0.set_frequency(self.tone_freq0)

    def set_tone_freq1(self, tone_freq1):
        self.tone_freq1 = tone_freq1
        self.analog_sig_source_x_1.set_frequency(self.tone_freq1)

    def set_b200mini_gain(self, b200mini_gain):
        self.b200mini_gain = b200mini_gain
        self.uhd_usrp_sink_0.set_gain(self.b200mini_gain, 0)

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.uhd_usrp_sink_0.set_center_freq(self.center_freq, 0)        

    def set_bb_ampl(self, bb_ampl):
        self.bb_ampl = bb_ampl
        self.analog_sig_source_x_0.set_amplitude(self.bb_ampl)

def main(argv):

    try:
       opts, args = getopt.getopt(argv,"hf:g:a:",["freq=", "db_gain=", "bb_ampl="])
    except getopt.GetoptError:
       print 'two_tone_power_measurement_B200mini.py -f <center_freq> -g <daughterboard_gain> -a <dac_scale>'
       sys.exit(2)

    for opt, arg in opts:
       if opt == '-h':
          print 'two_tone_power_measurement_B200mini.py -f <center_freq> -g <daughterboard_gain> -a <dac_scale>'
          sys.exit()
       elif opt in ("-f", "--freq"):
          center_freq = float(arg)
       elif opt in ("-g", "--db_gain"):
          b200mini_gain = float(arg)
       elif opt in ("-a", "--bb_ampl"):
          bb_ampl = float(arg)

    if center_freq < 70e6 or center_freq > 6e9:
       print 'Error! Invalid choice of center frequency for B200 mini.'
       print int(center_freq)
       sys.exit()

    if b200mini_gain < 0.0 or b200mini_gain > 89.8:
       print 'Error! Invalid choice of daughterboard gain for B200 mini.'
       sys.exit()

    if bb_ampl < 0.0 or bb_ampl > 1.0:
       print 'Error! Invalid choice of baseband signal scale factor.'
       sys.exit()

    dir_name = "./two_tone_power_measurements_B200mini"
    if not os.path.exists(dir_name):
       os.makedirs(dir_name)

    filename = "two_tone_power_B200mini_f" + str(int(center_freq)) + "_g" + str(int(b200mini_gain*10)) + "_a" + str(int(bb_ampl*100)) + ".bin"
    dir_filename = dir_name + "/" + filename    

    tb = top_block(center_freq, b200mini_gain, bb_ampl, dir_filename)    
    tb.start()    
    time.sleep(20)

    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main(sys.argv[1:])
