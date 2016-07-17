#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Communications Test
# Author: Jaiden Ferraccioli
# Generated: Thu Jun 16 21:01:02 2016
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
        self.samp_rate = samp_rate = 195312
        self.baud = baud = 9600
        self.samp_per_sym = samp_per_sym = samp_rate/baud

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("address0=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(460e6, 0)
        self.uhd_usrp_source_0.set_gain(0, 0)
        self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("address0=192.168.10.3", "")),
        	uhd.stream_args(
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
        self.Interfaces_queue_len_framer_source_b_0 = Interfaces.queue_len_framer_source_b('\x7e', True)
        self.Interfaces_queue_len_deframer_sink_b_0 = Interfaces.queue_len_deframer_sink_b('\x7e', True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Interfaces_queue_len_framer_source_b_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.digital_gfsk_demod_0, 0), (self.Interfaces_queue_len_deframer_sink_b_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.digital_gfsk_demod_0, 0))    

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(self.samp_rate/self.baud)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_samp_per_sym(self.samp_rate/self.baud)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

    def send(self, packet):
	self.Interfaces_queue_len_framer_source_b_0.send(packet)

    def receive(self):
	return self.Interfaces_queue_len_deframer_sink_b_0.receive()


def test_send(top_block_cls=com_test, options=None):

    tb = top_block_cls()
    tb.start()
    tb.send("Testing")
    try:
        raw_input('Press Enter to Transmit: ')
    except EOFError:
        pass
    tb.send("UBNL")
    tb.send("Communications")
    tb.send(" ")
    try:
	raw_input('Press Enter for Star Wars: ')
    except EOFError:
	pass
    tb.send("A long time ago in a galaxy far, far away....")
    tb.send("STAR WARS")
    tb.send(" ")
    tb.send("Turmoil has engulfed the Galactic Republic. The taxation of trade routes to outlying star systems is in dispute.")
    tb.send("Hoping to resolve the matter with a blockade of deadly battleships, the greedy Trade Federation has stopped all")
    tb.send("shipping to the small planet of Naboo. While the Congress of the Republic endlessly debates this alarming chain")
    tb.send("of events, the Supreme Chancellor has secretly dispatched two Jedi Knights, the guardians of peace and justice in")
    tb.send("the galaxy, to settle the conflict....")
    try:
	raw_input('Press Enter to Quit: ')
    except EOFError:
	pass
    tb.stop()
    tb.wait()


def main(top_block_cls=com_test, options=None):

	tb = top_block_cls()
	tb.start()
	print("Started")
	print("Packet 1: " + tb.receive())
	print("Packet 2: " + tb.receive())
	print("Packet 3: " + tb.receive())
	print("Packet 4: " + tb.receive())
	print("Packet 5: " + tb.receive())
	print("Packet 6: " + tb.receive())
	print("Packet 7: " + tb.receive())
	print("Packet 8: " + tb.receive())
	print("Packet 9: " + tb.receive())
	print("Packet 10: " + tb.receive())
	try:
		raw_input('Press Enter to quit: ')
	except EOFError:
		pass
	tb.stop()
	tb.wait()


if __name__ == '__main__':
    main()
