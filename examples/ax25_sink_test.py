#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: AX.25 Sink Test
# Author: Jaiden Ferraccioli
# Generated: Thu Jul 28 01:56:08 2016
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import Interfaces


class ax25_sink_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "AX.25 Sink Test")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 256000
        self.baud = baud = 9600
        self.samp_per_sym = samp_per_sym = samp_rate/baud

        ##################################################
        # Blocks
        ##################################################
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
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_char*1, "/home/ground/jaidenfe/gr-Interfaces/examples/.ax25_sink_test.dat", True)
        self.Interfaces_ax25_sink_b_0 = Interfaces.ax25_sink_b("WJ9XLE", "WJ9XLE")

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.digital_gfsk_demod_0, 0), (self.Interfaces_ax25_sink_b_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.digital_gfsk_demod_0, 0))    

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(self.samp_rate/self.baud)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_samp_per_sym(self.samp_rate/self.baud)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym


def main(top_block_cls=ax25_sink_test, options=None):

    tb = top_block_cls()
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
