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
#include "ax25_sink_b_impl.h"

namespace gr {
  namespace Interfaces {

    ax25_sink_b::sptr
    ax25_sink_b::make(char* src, char* dst)
    {
      return gnuradio::get_initial_sptr
        (new ax25_sink_b_impl(src, dst));
    }

    /*
     * The private constructor
     */
    ax25_sink_b_impl::ax25_sink_b_impl(char* src, char* dst)
      : gr::sync_block("ax25_sink_b",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(0, 0, 0))
    {}

    /*
     * Our virtual destructor.
     */
    ax25_sink_b_impl::~ax25_sink_b_impl()
    {
    }

    char* ax25_sink_b_impl::receive() {
	char* payload;
	// Get payload from receive queue
	return payload;
    }

    int
    ax25_sink_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Interfaces */
} /* namespace gr */

