#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Communications Test
# Author: Jaiden Ferraccioli
# Generated: Sun Jul 17 18:05:50 2016
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import Interfaces
import time

class com_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Communications Test")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000
        self.baud = baud = 9600
        self.samp_per_sym = samp_per_sym = samp_rate/baud

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	device_addr="address0=192.168.10.3",
        	stream_args=uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(460e6, 0)
        self.uhd_usrp_source_0.set_gain(0, 0)
        self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	device_addr="address0=192.168.10.2",
        	stream_args=uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(460e6, 0)
        self.uhd_usrp_sink_0.set_gain(0, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
        	samples_per_symbol=samp_per_sym,
        	sensitivity=1.0,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.digital_gfsk_demod_0 = digital.gfsk_demod(
        	samples_per_symbol=samp_per_sym,
        	sensitivity=1.0,
        	gain_mu=0.175,
        	mu=0.5,
        	omega_relative_limit=0.005,
        	freq_error=0.0,
        	verbose=False,
        	log=False,
        )
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.7, ))
        self.Interfaces_queue_len_framer_source_b_0 = Interfaces.queue_len_framer_source_b('U', True)
        self.Interfaces_queue_len_deframer_sink_b_0 = Interfaces.queue_len_deframer_sink_b('U', True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Interfaces_queue_len_framer_source_b_0, 0), (self.digital_gfsk_mod_0, 0))
        self.connect((self.digital_gfsk_demod_0, 0), (self.Interfaces_queue_len_deframer_sink_b_0, 0))
        self.connect((self.digital_gfsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.digital_gfsk_demod_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))


# QT sink close method reimplementation

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(self.samp_rate/self.baud)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_samp_per_sym(self.samp_rate/self.baud)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = com_test()
    tb.start()
    raw_input('Press Enter to quit: ')
    tb.stop()
    tb.wait()

