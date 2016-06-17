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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "queue_len_framer_source_b_impl.h"
#include <bitset>
#include <string>
#include <unistd.h>
#include <ctime>

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
    queue_len_framer_source_b_impl::queue_len_framer_source_b_impl(char preamble, bool txlog)
      : gr::sync_block("queue_len_framer_source_b",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
	_preamble = preamble;
	_log = txlog;
	_id_num = 0;
	_startup = std::time(NULL);
	state = BLOCKING;
	_samples_per_symbol = 26;
	_bits_per_symbol = 1;

	if(_log) {
		_log_file.open("tx_log", std::ios::out);

		if(!_log_file.is_open()) {

			std::cout << "Failed to open log file" << std::endl;
		}

		_log_file << "\n\nInitializing Radio Server Transmission..."
			  << "\nRadio Server Initialized: " << timestamp()
                          << "\n================================================="
                          << "\n[ Packet # ] [ Timestamp (Date|Time|Uptime) ] [   Payload   ]\n\n";
		_log_file.flush();
	}
    }



    /*
     * The virtual destructor
     */
    queue_len_framer_source_b_impl::~queue_len_framer_source_b_impl() {
	if(_log) {
		_log_file.close();
	}
    }



    /*
     * The send function
     */
    void queue_len_framer_source_b_impl::send(char * packet) {

	_phy_i.push(packet);
	d_not_empty.notify_all();
    }



    /*
     * Conversion between integer and byte
     */
    char queue_len_framer_source_b_impl::itob(int num) {

	return (static_cast<char>(std::bitset<8>(num).to_ulong()));
    }



    /*
     * Timestamp fucntion
     */
    const std::string queue_len_framer_source_b_impl::timestamp() {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }


    /*
     * Uptime function
     */
    std::time_t queue_len_framer_source_b_impl::uptime() {
	return ((std::time(NULL)) - _startup);
    }



    /*
     * Blocking Function
     */
    void queue_len_framer_source_b_impl::blocking() {

	gr::thread::scoped_lock lock(q_mutex);
	while( _phy_i.empty() ) d_not_empty.wait(lock);
    }



    int queue_len_framer_source_b_impl::gcd(int a, int b) {

	if(b == 0) {
		return a;
	}
	else {
		return gcd(b, (a % b));
	}
    }



    int queue_len_framer_source_b_impl::lcm(int a, int b) {

	return ((a * b)/gcd(a,b));
    }



    int queue_len_framer_source_b_impl::n_padding_bytes() {

	int byte_mod = lcm(16, _samples_per_symbol) * _bits_per_symbol/_samples_per_symbol;
	int r = _pac_len %byte_mod;
	if (r == 0) return 0;
	return byte_mod - r;
    }



    /*
     * The work function
     * Where all the magic happens
     */
    int queue_len_framer_source_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items) {

	char *out = (char *) output_items[0];

	int i = 0;
	int size = noutput_items;
	int pad;

	while(size) {
		switch(state) {

			case BLOCKING:
				blocking();
				_packet = _phy_i.front();
				_phy_i.pop(); // Maybe pop packet once it is properly sent?
				out[i] = '\x00';
				i++;
				size--;
				out[i] = _preamble;
				state = FRAME_PREAMBLE;
				i++;
				break;

			case FRAME_PREAMBLE:
				out[i] = _preamble;
				state = FRAME_LENGTH;
				i++;
				break;

			case FRAME_LENGTH:
				_pac_len = strlen(_packet);
				out[i] = itob(_pac_len);
				state = FRAME_PAYLOAD;
				i++;
				_hold = i;
				break;

			case FRAME_PAYLOAD:
				out[i] = _packet[i - _hold];
				if((i - _hold) == _pac_len) {
					state = PADDING;
					if(_log) {
						_log_file << std::setfill('0')
							<< std::setw(12) << _id_num
							<< " [" << timestamp()
							<< " " << std::setfill('0')
							<< std::setw(10) << uptime()
							<< "] "<< _packet << "\n";
						_log_file.flush();
					}
					out[i+1] = '\x00';
					out[i+2] = '\x55';
					// check this...
					_id_num++;
					size = size - 3;
					i = i + 3;
					break;
				}

				i++;
				break;

			case PADDING:
				pad = n_padding_bytes();
				for(int v=0; v < pad; v++) {
					out[i] = '\x55';
					i++;
					size--;
				}
				state = BLOCKING;
				return noutput_items - size;
				// Pad

			default:
				state = BLOCKING;
				break;

		}

		size--;
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
    }

  } /* namespace Interfaces */
} /* namespace gr */

