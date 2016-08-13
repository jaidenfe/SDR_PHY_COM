#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Ax25 Rx Test
# Author: Jaiden Ferraccioli
# Generated: Tue Aug  2 18:35:26 2016
##################################################

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import Interfaces
import time


class ax25_rx_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Ax25 Rx Test")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 195312

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
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(145.9e6, 0)
        self.uhd_usrp_source_0.set_gain(0, 0)
        self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
        self.digital_gfsk_demod_0 = digital.gfsk_demod(
        	samples_per_symbol=2,
        	sensitivity=1.0,
        	gain_mu=0.175,
        	mu=0.5,
        	omega_relative_limit=0.005,
        	freq_error=0.0,
        	verbose=False,
        	log=False,
        )
        self.Interfaces_ax25_sink_b_0 = Interfaces.ax25_sink_b("WJ9XLE", "WJ9XLE", True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.digital_gfsk_demod_0, 0), (self.Interfaces_ax25_sink_b_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.digital_gfsk_demod_0, 0))    

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def receive(self):
        return self.Interfaces_ax25_sink_b_0.receive()


def main(top_block_cls=ax25_rx_test, options=None):

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
