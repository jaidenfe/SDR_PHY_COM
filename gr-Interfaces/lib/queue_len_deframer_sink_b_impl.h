/* -*- c++ -*- */
/*
 * Copyright 2016 University at Buffalo Nanosatellite Laboratory.
 * Author: Jaiden Ferraccioli
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

#ifndef INCLUDED_INTERFACES_QUEUE_LEN_DEFRAMER_SINK_B_IMPL_H
#define INCLUDED_INTERFACES_QUEUE_LEN_DEFRAMER_SINK_B_IMPL_H

#include <Interfaces/queue_len_deframer_sink_b.h>
#include <queue>
//#include <condition_variable>
#include <vector>
#include <bitset>

namespace gr {
  namespace Interfaces {

    class queue_len_deframer_sink_b_impl : public queue_len_deframer_sink_b {
	private:

		static const int TIMEOUT = 100;

		// States
		enum State {
			POPULATE_BUFFER = 0,
			DETERMINE_PREAMBLE = 1,
			DETERMINE_LENGTH = 2,
			DETERMINE_PACKET = 3
		} state;

		// Initialize Private Variables
		std::queue<char *> _phy_i;
		//std::condition_variable _cv;
		int _pac_len;
		char _preamble;
		bool _log; // Implement the log functionality later on

		// Initialize Buffers
		std::vector<char> detect;
		std::vector<char> v_length;
		std::vector<char> byte_buffer;
		std::vector<char> packet;

		// Conversion Functions
		int ubvtoi(std::vector<char> v_len);
		char pack(std::vector<char> ubyte);
		bool check_preamble(std::vector<char> ub_in);

	public:
		queue_len_deframer_sink_b_impl(char preamble, bool rxlog);
		~queue_len_deframer_sink_b_impl();

		char * receive();
		int work(int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
    };

  } // namespace Interfaces
} // namespace gr

#endif /* INCLUDED_INTERFACES_QUEUE_LEN_DEFRAMER_SINK_B_IMPL_H */

