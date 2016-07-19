#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: ax25_tx_test
# Author: Jaiden Ferraccioli
# Description: Testing AX.25 encoding and crc calucaltions.
# Generated: Fri Jul  1 16:28:38 2016
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


class top_block(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "ax25_tx_test")

        ##################################################
        # Variables
        ##################################################
        self.tx_samp_rate = tx_samp_rate = 233645
        self.baud = baud = 9600
        self.tx_samp_per_sym = tx_samp_per_sym = tx_samp_rate/baud
        self.tx_freq = tx_freq = 450e6

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(tx_samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(tx_freq, 0)
        self.uhd_usrp_sink_0.set_gain(10, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
        	samples_per_symbol=tx_samp_per_sym,
        	sensitivity=1.0,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.8, ))
        self.Interfaces_ax25_queue_source_b_0 = Interfaces.ax25_queue_source_b('U', True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Interfaces_ax25_queue_source_b_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))    

    def get_tx_samp_rate(self):
        return self.tx_samp_rate

    def set_tx_samp_rate(self, tx_samp_rate):
        self.tx_samp_rate = tx_samp_rate
        self.set_tx_samp_per_sym(self.tx_samp_rate/self.baud)
        self.uhd_usrp_sink_0.set_samp_rate(self.tx_samp_rate)

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_tx_samp_per_sym(self.tx_samp_rate/self.baud)

    def get_tx_samp_per_sym(self):
        return self.tx_samp_per_sym

    def set_tx_samp_per_sym(self, tx_samp_per_sym):
        self.tx_samp_per_sym = tx_samp_per_sym

    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.uhd_usrp_sink_0.set_center_freq(self.tx_freq, 0)

    def send(self, packet):
        self.Interfaces_ax25_queue_source_b_0.send(packet)


def main(top_block_cls=top_block, options=None):

    tb = top_block_cls()
    tb.send("Hello World")
    tb.start()
    try:
        raw_input('Press Enter to Send: ')
    except EOFError:
        pass
    tb.send("Testing")
    tb.send("Communications")
    try:
        raw_input('Press Enter to Quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()



if __name__ == '__main__':
    main()
