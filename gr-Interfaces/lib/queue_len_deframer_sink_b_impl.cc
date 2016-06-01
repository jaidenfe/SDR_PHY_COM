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
#include "queue_len_deframer_sink_b_impl.h"

namespace gr {
  namespace Interfaces {

    queue_len_deframer_sink_b::sptr
    queue_len_deframer_sink_b::make(char preamble, bool rxlog)
    {
      return gnuradio::get_initial_sptr
        (new queue_len_deframer_sink_b_impl(preamble, rxlog));
    }

    /*
     * The private constructor
     */
    queue_len_deframer_sink_b_impl::queue_len_deframer_sink_b_impl(char preamble, bool rxlog)
      : gr::sync_block("queue_len_deframer_sink_b",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(0, 0, 0))
    {
      // Initialize the private variables and state whether or not to log received information 
      _preamble = preamble;
      _log = rxlog;
    }

    /*
     * Our virtual destructor.
     */
    queue_len_deframer_sink_b_impl::~queue_len_deframer_sink_b_impl() {}

    char * queue_len_deframer_sink_b_impl::receive() {

      /*TO-DO:

        This is where we define the receiving functionality for the higher layers 
        and for conversion to Phython using SWIG. Here just access the first element 
        and remove it from the queue. Then return the char pointer (will be a string in python)
      */
	if(_phy_i.empty()) {
		_cv.wait();
	}

	// Access and store the first received packet and remove it from the queue
	char * packet = _phy_i.front();
	_phy_i.pop();

	// Return first received packet
	return packet;
    }

    int queue_len_deframer_sink_b_impl::work(int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items) {

      const char *in = (const char *) input_items[0];

      // Do <+signal processing+>

      /* TO-DO:

      Here is where we need to look through the UNPACKED
      bytes (each bit is a byte (char) that is one input)
      for the preamble that occurs twice. After this is found
      we get the next 8 input bytes (chars) and repack them
      (turn them back into a single bit). From here we turn this
      packed byte into a number (between 0 and 256) and multiply it
      by 8. We have to do this because we are dealing with unpacked bytes.
      We then use this number to receive the next n number of unpacked
      bytes, packe them, parse them into packed bytes and turn each byte
      into a char. From here we add them to a char pointer and push them
      on to the queue for higher layers to retreive. When a packet is added
      to the queue call notify_all() on the conditional variable '_cv'.

      */

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Interfaces */
} /* namespace gr */

