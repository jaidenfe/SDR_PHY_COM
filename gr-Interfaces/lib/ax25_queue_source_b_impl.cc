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
#include "ax25_queue_source_b_impl.h"
#include <bitset>
#include <string>
#include <unistd.h>
#include <ctime>

namespace gr {
  namespace Interfaces {

    ax25_queue_source_b::sptr
    ax25_queue_source_b::make(char preamble, bool txlog)
    {
      return gnuradio::get_initial_sptr
        (new ax25_queue_source_b_impl(preamble, txlog));
    }

    /*
     * The private constructor
     */
    ax25_queue_source_b_impl::ax25_queue_source_b_impl(char preamble, bool txlog)
      : gr::sync_block("ax25_queue_source_b",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
	_preamble = preamble;
	_log = txlog;
	_id_num = 0;
	_startup = std::time(NULL);
	state = BLOCKING;
	_samples+per_symbol = 26;
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
     * Our virtual destructor.
     */
    ax25_queue_source_b_impl::~ax25_queue_source_b_impl()
    {
	if(_log) {
                _log_file.close();
        }
    }

    /*
     * The send function
     */
    void ax25_queue_source_b_impl::send(char * packet) {

        _phy_i.push(packet);
        d_not_empty.notify_all();
    }


    /*
     * Conversion between integer and byte
     */
    char ax25_queue_source_b_impl::itob(int num) {

        return (static_cast<char>(std::bitset<8>(num).to_ulong()));
    }



    /*
     * Timestamp fucntion
     */
    const std::string ax25_queue_source_b_impl::timestamp() {
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
    std::time_t ax25_queue_source_b_impl::uptime() {
        return ((std::time(NULL)) - _startup);
    }

    /*
     * Blocking Function
     */
    void ax25_queue_source_b_impl::blocking() {

        gr::thread::scoped_lock lock(q_mutex);
        while( _phy_i.empty() ) d_not_empty.wait(lock);
    }



    int ax25_queue_source_b_impl::gcd(int a, int b) {

        if(b == 0) {
                return a;
        }
        else {
                return gcd(b, (a % b));
        }
    }



    int ax25_queue_source_b_impl::lcm(int a, int b) {

        return ((a * b)/gcd(a,b));
    }



    int ax25_queue_source_b_impl::n_padding_bytes() {

        int byte_mod = lcm(16, _samples_per_symbol) * _bits_per_symbol/_samples_per_symbol;
        int r = _pac_len %byte_mod;
        if (r == 0) return 0;
        return byte_mod - r;
    }

    char* ax25_queue_source_b_impl::bit_stuff(char* pac) {
	int sim = 0;
	int r = 0;
	int t_len = 0;
	char* ret;
	char* rret;
	for(int i = 0; i < _pac_len; i++) {
		for(int b = 0; b < 8; b++) {
			ret[r] = ((pac[i] >> b) & 1);
			t_len++;
			if (ret[r] == '\x01') {
				sim++;
			}
			if(ret[r] == '\x00') {
				sim = 0;
			}
			if(sim == 5) {
				r++;
				ret[r] = '\x00';
				t_len++;
			}
			r++;
		}
`	}
	// Repack the bit-stuffed payload
	int rp = 7;
	int t = 0;
	for(int j = 0; j < t_len; j++) {
		if(rp == 0) {
			rp = 7;
			t++;
		}
		rret[t] = (ret[j] << rp) & rret[t];
		rp--;
	}
	return rret;
    }








// IMPLEMENT THE FCS CHECKSUM FIELD DESCRIBED BELOW IN C


/* Table of CRCs of all 8-bit messages. */
   unsigned long crc_table[256];

   /* Flag: has the table been computed? Initially false. */
   int crc_table_computed = 0;

   /* Make the table for a fast CRC. */
   void make_crc_table(void)
   {
     unsigned long c;
     int n, k;

     for (n = 0; n < 256; n++) {
       c = (unsigned long) n;
       for (k = 0; k < 8; k++) {
         if (c & 1)
           c = 0xedb88320L ^ (c >> 1);
         else
           c = c >> 1;
       }
       crc_table[n] = c;
     }
     crc_table_computed = 1;
   }



    unsigned long ax25_queue_source_b_impl::update_crc(unsigned long crc, unsigned char *buff, int len) {
	unsigned long c = crc;
	int n;
	if(!crc_table_computed){
		make_crc_table();
	}
	for(n=0; n<len; n++) {
		c = crc_table[(c^buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
    }

    std::vecotr<char> ax25_queue_source_b_impl::calc_fcs(char* output, int length) {
	std::vector<char> ret;
	unsigned long crc_calc;
	crc_calc = update_crc(0xffffffffL, output, length) ^ 0xffffffffL;

	ret[0] = ;
	ret[1] = ;
	return ret;
    }

    int ax25_queue_source_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      char *out = (char *) output_items[0];

      int i = 0;
        int size = noutput_items;
        int pad;

        while(size) {
                switch(state) {

                        case BLOCKING:
                                blocking();
                                _packet = _phy_i.front();
                                _phy_i.pop();
                                out[i] = '\x00';
                                i++;
                                size--;
                                out[i] = '\x7e';
                                state = ADDRESS_FIELD;
                                i++;
                                break;

                        case ADDRESS_FIELD:
                                out[i] = 'W';
				out[i+1] = 'J';
				out[i+2] = '9';
				out[i+3] = 'X';
				out[i+4] = 'L';
				out[i+5] = 'E';
				out[i+6] = '\xe0'; // SSID #1
				out[i+7] = 'W';
				out[i+8] = 'J';
				out[i+9] = '9';
				out[i+10] = 'X';
				out[i+11] = 'L';
				out[i+12] = 'E';
				out[i+13] = '\x61'; // SSID #2
                                state = CONTROL_FIELD;
                                i+=14;
                                break;

                        case CONTROL_FIELD:
                                out[i] = '\x03'; // Control Field for UI Frame with no immediate responce required (Poll/Final bit set to zero)
				out[i+1] = '\xf0'; // No Layer 3 Protocol Implemented
                                state = INFO_FIELD;
                                i+=2;
				_hold = i;
				_packet = bit_stuff(_packet); // every 5 1 bits in a row add a zero after
                                break;

                       case INFO_FIELD:
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
					std::vecotr<char> fcs = calc_fcs()
                                        out[i+1] = fcs[0];
                                        out[i+2] = fcs[1];
					out[i+3] = '\x7e'; // Ending Flag
                                        // check this...
                                        _id_num++;
                                        size = size - 4;
                                        i = i + 4;
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

