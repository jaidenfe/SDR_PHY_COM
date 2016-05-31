/* -*- c++ -*- */
/*
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "queue_len_framer_source_b_impl.h"
#include <bitset>
#include <string>

namespace gr {
  namespace Interfaces {

    queue_len_framer_source_b::sptr
    queue_len_framer_source_b::make(char preamble, bool txlog)
    {
      return gnuradio::get_initial_sptr
        (new queue_len_framer_source_b_impl(preamble, txlog));
    }

    /*
     * The private constructor
     */
    queue_len_framer_source_b_impl::queue_len_framer_source_b_impl(char preamble = 'U', bool txlog = true)
      : gr::sync_block("queue_len_framer_source_b",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
	_preamble = preamble;
	_log = txlog;
    }

    /*
     * Our virtual destructor.
     */
    queue_len_framer_source_b_impl::~queue_len_framer_source_b_impl() {}

    void queue_len_framer_source_b_impl::send(char * packet) {

	_phy_i.push(packet);
    }

    char queue_len_framer_source_b_impl::itob(int num) {

	return (static_cast<char>(std::bitset<8>(num).to_ulong()));
    }

    int queue_len_framer_source_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {

      char *out = (char *) output_items[0];

      if(!_phy_i.empty()) {
		// Get top packet from queue and remove it
		char * packet = _phy_i.front();
		_phy_i.pop();

		// Add preamble to output buffer
		out[0] = _preamble;
		out[1] = _preamble;

		// Add length of packet to output buffer
		int length = strlen(packet);
		out[2] = itob(length);

		// Add packet data to output buffer
		for (int i = 0; i < length; i++) {

			out[i+3] = packet[i];
		}

		if (_log) {

			std::string output = out;
			std::cout << "-------------------------------------" << std::endl;
			std::cout << "\nSuccessfully Transmitted Packet" << std::endl;
			std::cout << "\nTime: " << std::endl;
			std::cout << "Length: " << length << std::endl;
			std::cout << "Packet: " << output << std::endl;
			std::cout << "------------------------------------" << std::endl;
		}
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Interfaces */
} /* namespace gr */

