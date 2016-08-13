/* -*- c++ -*- */
/*
 * Author: Jaiden Ferraccioli
 * Copyright 2016 University at Buffalo Nanosatellite Laboratory
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_INTERFACES_AX25_SINK_B_IMPL_H
#define INCLUDED_INTERFACES_AX25_SINK_B_IMPL_H

#include <Interfaces/ax25_sink_b.h>
#include <queue>
#include <vector>
#include <string>
#include <fstream>

#define FLAG '\x7e'


namespace gr {
  namespace Interfaces {

	class ax25_sink_b_impl : public ax25_sink_b {
	    private:

		std::ofstream _log_file;

		// Initialize  States
		enum State {
			POPULATE_BUFFER = 0,
			DETERMINE_START_FLAG = 1,
			DETERMINE_END_FLAG = 2,
			UNSTUFF_PACKET = 3,
			DIFFERENTIAL_DECODE = 4,
			DESCRAMBLE = 5,
			CALCULATE_CRC16 = 6
		} state;

		// Initialize Buffers
		std::queue<char *> _phy_i;
		std::queue<unsigned char> detect;
		std::queue<unsigned char> raw_packet;
		std::queue<unsigned char> ppacket;
		std::vector<unsigned char> decode_packet;
		char* packet_t;
		char* packet;

		// Initialize Private Variables
		bool _log;
		bool _debug;
		time_t _startup;
		char* _addr_src;
		char* _addr_dst;
		char t;
		unsigned short packet_crc;
		double _id_num;
		size_t num;
		size_t i;
		unsigned char last;
		unsigned char newbit;
		uint32_t shift_register;
		uint32_t mask;
		uint32_t seed;
		uint32_t shift_register_length;

		// Conversion Functions
		const std::string timestamp();
		std::time_t uptime();
		bool check_flag(std::queue<unsigned char> qin);
		unsigned short crc16_ccitt(char *buf, int len);
		uint32_t popCount(uint32_t x);

	    public:

		ax25_sink_b_impl(char* src, char* dst, bool log);
		~ax25_sink_b_impl();

		char * receive();

		int work(int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
	};

  } // namespace Interfaces
} // namespace gr

#endif /* INCLUDED_INTERFACES_AX25_SINK_B_IMPL_H */

