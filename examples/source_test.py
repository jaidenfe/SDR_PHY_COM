#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Queue Length-Based Framer Source Test
# Author: Jaiden Ferraccioli
# Generated: Tue Jun 14 01:38:00 2016
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import Interfaces


class queue_framer_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Queue Length-Based Framer Source Test")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 256000
        self.baud = baud = 9600
        self.samp_per_sym = samp_per_sym = samp_rate/baud

        ##################################################
        # Blocks
        ##################################################
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, "/home/ubnl-sof/COM/SDR_PHY_COM/tests/.source_test.dat", False)
        self.blocks_file_sink_0.set_unbuffered(True)
        self.Interfaces_queue_len_framer_source_b_0 = Interfaces.queue_len_framer_source_b('\x33', True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Interfaces_queue_len_framer_source_b_0, 0), (self.blocks_file_sink_0, 0))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(self.samp_rate/self.baud)

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

def main(top_block_cls=queue_framer_test, options=None):

    tb = top_block_cls()
    tb.send("Hello World")
    tb.start()
    tb.send("Testing")
    tb.send("UBNL")
    tb.send("Communications")
    try:
        raw_input('Press Enter to Send: ')
    except EOFError:
	pass
    tb.send("Testing")
    tb.send("Communications")
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.send("Eureka!")
    try:
	raw_input('Press Enter to Quit: ')
    except EOFError:
	pass
    tb.stop()
    tb.wait()

if __name__ == '__main__':
    main()
