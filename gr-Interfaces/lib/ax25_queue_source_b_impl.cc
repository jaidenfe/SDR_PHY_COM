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
#include <stdio.h>

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
	_samples_per_symbol = 24;
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


    /*
     * Greatest Common Denominator Function
     */
    int ax25_queue_source_b_impl::gcd(int a, int b) {

        if(b == 0) {
                return a;
        }
        else {
                return gcd(b, (a % b));
        }
    }


    /*
     * Least Common Multiple Function
     */
    int ax25_queue_source_b_impl::lcm(int a, int b) {

        return ((a * b)/gcd(a,b));
    }


    /*
     * Padding Function for USRP
     */
    int ax25_queue_source_b_impl::n_padding_bytes() {

        int byte_mod = lcm(16, _samples_per_symbol) * _bits_per_symbol/_samples_per_symbol;
        int r = _pac_len %byte_mod;
        if (r == 0) return 0;
        return byte_mod - r;
    }









    /*
     * Bit Stuffing Function
     * Re-write this function
     */
    void ax25_queue_source_b_impl::bit_stuff(char* pac) {
	// Implement this stupid bit stuffing bullshit
	char unpacked[2464];
	char packed[308];
	int count = 0;
	int unpack_len = 0;
	int pack_len = 0;
	int pos = 0;
	_pac_len = strlen(pac);
	// Populate Unpacked Byte Buffer
	for(int i = 0; i < _pac_len; i++) {
		for(int j = 0; j < 8; j++) {
			char a = ((pac[i] >> j) & '\x01');
			if(a == '\x01') count++;
			else count = 0;
			unpacked[pos] = a;
			unpack_len++;
			pos++;
			if(count == 5){
				count = 0;
				unpacked[pos] = '\x00';
				unpack_len++;
				pos++;
			}
		}
	}

	// Make sure unpacked byte buffer is a multiple of 8
	// and zero stuff if it is not
	while((unpack_len%8) != 0){
		unpacked[pos] = '\x00';
		unpack_len++;
		pos++;
	}

	// Repack the byte buffer and memcpy it into a char*
	int l = 0;
	int s = 7;
	char t = '\x00';
	for(int k=0; k < unpack_len; k++) {

		if(s == 0) {
			packed[l] = t;
			s = 7;
			t = '\x00';
			l++;
			pack_len++;
		}

		t = (unpacked[k] << s) & t;
		s--;
	}
	//_packet = &packed; // _packet is a char* to point to a char array (string)

	for(int n = 0; n < pack_len; n++){
		_packet[n] = packed[n];
		std::cout << packed[n] << "\n" << std::endl;
	}
	std::cout << "Unpack Length: " << unpack_len << std::endl;
	std::cout << "Pack Length: " << pack_len << std::endl;
	//memcpy(_packet, packed, pack_len);

    }




    /*
     * FCS Field Calculation Function
     */
    short unsigned int ax25_queue_source_b_impl::calculate_crc(char* buf, int len){
	short unsigned int crc = 0xffff;
	for(int i = 0; i < len; i++){
		for(int j = 0; j < 8; j++){
			if((crc & 0x0001) != ((buf[i] >> j) & 0x01)){
				crc = (crc >> 1) ^ 0x8408;
			}
			else {
				crc = crc >> 1;
			}
		}
	}
	crc = crc ^ 0xffff;
	return crc;
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
				std::cout << "Blocking" << std::endl;
                                blocking();
				delete[] _packet;
                                bit_stuff(_phy_i.front());
				_pac_len = std::strlen(_packet);
                                _phy_i.pop();
                                out[i] = '\x00';
                                i++;
                                size--;
                                out[i] = '\x7e';
                                state = ADDRESS_FIELD;
				size--;
                                i++;
                                break;

                        case ADDRESS_FIELD:
				std::cout << "Address Field" << std::endl;
                                out[i] = 'W';
				out[i+1] = 'J';
				out[i+2] = '9';
				out[i+3] = 'X';
				out[i+4] = 'L';
				out[i+5] = 'E';
				out[i+6] = '\x01'; // SSID #1
				out[i+7] = 'W';
				out[i+8] = 'J';
				out[i+9] = '9';
				out[i+10] = 'X';
				out[i+11] = 'L';
				out[i+12] = 'E';
				out[i+13] = '\x02'; // SSID #2
                                state = CONTROL_FIELD;
				size= size-14;
                                i = i+14;
                                break;

                        case CONTROL_FIELD:
				std::cout << "Control Field" << std::endl;
                                out[i] = '\x03'; // Control Field for UI Frame with no immediate responce required (Poll/Final bit set to zero)
				out[i+1] = '\xf0'; // No Layer 3 Protocol Implemented
                                state = INFO_FIELD;
				size=size-2;
                                i=i+2;
				_hold = i;
                                break;

                       case INFO_FIELD:
				std::cout << "Information Field" << std::endl;
				//std::cout << "Why aren't you working?" << std::endl;
				//std::cout << "i: " << i;
				//std::cout << "\nhold: " << _hold << std::endl;
				//std::cout << "Packet: " << _packet[0] << std::endl;
				//std::cout << "Output: " << out[i] << std::endl;
                                out[i] = _packet[i - _hold];
  				//_packet bit_stuff(_packet, out, i);
				//std::cout << "Pre IF Statement" << std::endl;
                                if((i - _hold) == _pac_len) {
                                        state = PADDING;
                                        if(_log) {
                                                _log_file << std::setfill('0')
                                                        << std::setw(12) << _id_num
                                                        << " [" << timestamp()
                                                        << " " << std::setfill('0')
                                                        << std::setw(10) << uptime()
                                                        << "] "<< _packet << "\nRaw Packet: ";
							int c = 0;
							while (c != i){
								_log_file << out[c];
								c++;
							}
						_log_file << "\n";
                                                _log_file.flush();
                                        }

					std::cout << "Pre-FCS" << std::endl;

					short unsigned int fcs;
					fcs = calculate_crc(out, std::strlen(out));
					std::cout << "CRC: " << fcs << std::endl;

					// Get the two bits out of the short
					out[i+1] = '\x00';
					out[i+2] = '\x00';
					out[i+3] = '\x7e'; // Ending Flag
                                        // check this...
                                        _id_num++;
                                        size -= 3;
                                        i += 3;
                                        break;
                                }
				//std::cout << "Post IF Statement" << std::endl;
				size--;
                                i++;
                                break;

                        case PADDING:
				std::cout << "Padding" << std::endl;
                                pad = n_padding_bytes();
                                for(int v=0; v < pad; v++) {
                                        out[i] = '\x55';
                                        i++;
                                        size--;
                                }
				state = BLOCKING;
                                return noutput_items - size;

                        default:
                                state = BLOCKING;
                                break;

                }

        }


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Interfaces */
} /* namespace gr */

