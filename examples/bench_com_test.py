#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Bench Com Test
# Author: Jaiden Ferraccioli
# Description: Full duplex physical layer communication node to test communication between various radios and custom GNU Radio system interface
# Generated: Tue Jul 19 04:45:13 2016
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


class bench_com_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Bench Com Test")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 195312
        self.baud = baud = 9600
        self.tx_freq = tx_freq = 430e6
        self.samp_per_sym = samp_per_sym = samp_rate/baud
        self.rx_freq = rx_freq = 430e6
        self.gain = gain = 0
        self.bandwidth = bandwidth = 10
        self.tx_addr = "192.168.10.3"
        self.rx_addr = "192.168.10.2"

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("addr=192.168.10.3", "addr=192.168.10.2")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(rx_freq, 0)
        self.uhd_usrp_source_0.set_gain(gain, 0)
        self.uhd_usrp_source_0.set_antenna("RX2", 0)
        self.uhd_usrp_source_0.set_bandwidth(bandwidth, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.10.2", "addr=192.168.10.3")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(tx_freq, 0)
        self.uhd_usrp_sink_0.set_gain(gain, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_bandwidth(bandwidth, 0)
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
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.8, ))
        self.Interfaces_length_framing_source_b_0 = Interfaces.length_framing_source_b("GP", True)
        self.Interfaces_length_framing_sink_b_0 = Interfaces.length_framing_sink_b("GP", True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Interfaces_length_framing_source_b_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.digital_gfsk_demod_0, 0), (self.Interfaces_length_framing_sink_b_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.digital_gfsk_demod_0, 0))    

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

    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.uhd_usrp_sink_0.set_center_freq(self.tx_freq, 0)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

    def get_rx_freq(self):
        return self.rx_freq

    def set_rx_freq(self, rx_freq):
        self.rx_freq = rx_freq
        self.uhd_usrp_source_0.set_center_freq(self.rx_freq, 0)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.uhd_usrp_source_0.set_gain(self.gain, 0)
        	
        self.uhd_usrp_sink_0.set_gain(self.gain, 0)
        	

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth
        self.uhd_usrp_source_0.set_bandwidth(self.bandwidth, 0)
        self.uhd_usrp_sink_0.set_bandwidth(self.bandwidth, 0)

    def send(self, packet):
        self.Interfaces_length_framing_source_b_0.send(packet)

    def receive(self):
        return self.Interfaces_length_framing_sink_b_0.receive()

def test_send(top_block_cls=bench_com_test, options=None):

    tb = top_block_cls()
    tb.start()
    while True:
        pac = raw_input("Packet: ")
        if pac == exit:
            tb.stop()
            del tb
            break
        tb.send(pac)
    '''
    try:
        raw_input('Press Enter to Transmit: ')
    except EOFError:
        pass
    tb.send("UBNL")
    tb.send("Communications")
    tb.send(" ")
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
    del tb
    '''

def test_receive(top_block_cls=bench_com_test, options=None):

        tb = top_block_cls()
        tb.start()
	n = 0
        print("Started")
	while True:
            print("Packet " + str(n) + ": " + tb.receive())
            n += 1

def main():
	print("    ==========================================================")
	print("                         BENCH COM TEST")
	print("    ==========================================================\n")
	print("    This program is intended to test the functionality of the\033[0m")
	print("    g-Interfaces/Framing submodule blocks. At least one Ettus\033[0m")
	print("    Research USRP N210 is required for this test. Make sure\033[0m")
	print("    the SDRs are configured properly and connected via Gigabit\033[0m")
	print("    Ethernet connection (including Gigabit Switch if multiple\033[0m")
	print("    SDRs are connected. Make sure that the connected devices\033[0m")
	print("    have the proper IP addresses (192.168.10.3 for transmission\033[0m")
	print("    and 192.168.10.2 for receiving). Alternatively, set the UHD\033[0m")
	print("    source/sink block 'device address' field to match your N210\033[0m")
	print("    IP address\033[0m\n")

	while True:
		cmd = raw_input('\033[92mRadio Operation(tx/rx)\033[0m: ')
		if cmd == 'tx':
			test_send()
			pass
		elif cmd == 'rx':
			test_receive()
			pass
		elif cmd == 'info':
			txf = "145.9 MHz"
			rxf = "430 MHz"
			txa = "\033[91mdisconnected\033[0m"
			rxa = "\033[91mdisconnected\033[0m"
			c = True
			try:
				bc = bench_com_test()
				txf = str(bc.tx_freq) + " Hz" 
				txa = str(bc.tx_addr)
				rxf = str(bc.rx_freq) + " Hz"
				rxa = str(bc.rx_addr)

			except RuntimeError:
				print("\n    \033[91mError:\033[0m No UHD Device Found\n")
				c = False
				pass
			print("    \033[1;1mTransmission\033[0m")
			print("       \033[35mTx Frequency\033[0m: " + str(txf))
			print("       \033[35mTx Device Address\033[0m: " + str(txa))
			print("\n    \033[1;1mReceiving\033[0m")
			print("       \033[35mRx Frequency\033[0m: " + str(rxf))
			print("       \033[35mRX Device Address\033[0m: " + str(rxa))
			print(" ")
			if c is True:
				del bc
		elif cmd == 'exit' or cmd == 'q' or cmd == 'quit':
			break
		else:
			print("\n    \033[91mError:\033[0m invalid user input\n\nPlease type '\033[36mtx\033[0m' to test transmission or '\033[36mrx\033[0m' to test receiving\nType '\033[36minfo\033[0m' for information about the program")
			print("    Type '\033[36mq\033[0m', '\033[36mquit\033[0m' or '\033[36mexit\033[0m' to quit\n")
			pass

if __name__ == '__main__':
    main()
