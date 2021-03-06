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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "ax25_sink_b_impl.h"
#include <bitset>

namespace gr {
  namespace Interfaces {


	ax25_sink_b::sptr
	ax25_sink_b::make(char* src, char* dst, bool log)
	{
	    return gnuradio::get_initial_sptr
		(new ax25_sink_b_impl(src, dst, log));
	}

	/*
	 * The private constructor
	 */
	ax25_sink_b_impl::ax25_sink_b_impl(char* src, char* dst, bool log)
	    : gr::sync_block("ax25_sink_b",
		gr::io_signature::make(1, 1, sizeof(char)),
		gr::io_signature::make(0, 0, 0))
   	{

		_addr_src = src;
		_addr_dst = dst;
		_startup = time(0);
		_log = log;
		_debug = true;
		_id_num = 0;
		mask = '\x21'; // may be some version of 0x21
		seed = '\x00'; // may be some version of 0x0
		shift_register_length = 16;

		if(_log) {
			_log_file.open("rx_log", std::ios::out);
	                if(!_log_file.is_open()) {
        	                std::cout << "Failed to open log file" << std::endl;
			}
			_log_file << "\n\nInitializing Radio Server Receiving..."
				  << "\nRadio Server Initialized: " << timestamp()
				  << "\n================================================="
				  << "\n[ Packet # ] [ Timestamp (Date|Time|Uptime) ] [   Payload   ]\n\n";
			_log_file.flush();
		}

		state = POPULATE_BUFFER;
	}

	/*
	 * The virtual destructor
	 */
	ax25_sink_b_impl::~ax25_sink_b_impl() {

		if(_log) {
			_log_file.close();
		}
	}

	/*
	 * Constructs a timestamp for logging and returns it as a string
	 */
	const std::string ax25_sink_b_impl::timestamp() {

		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

		return buf;
	}

	/*
	 * Returns true if the buffer matches the FLAG value
	 */
	bool ax25_sink_b_impl::check_flag(std::queue<unsigned char> qin) {
		std::bitset<8> bit_buffer;

		unsigned char out;

		for(int i=0; i < 16; i++) {
			unsigned char bit_in = qin.front();
			qin.pop();
			if(i < 8) {
				bit_buffer.set((7-i), bit_in);
			}
		}

		out = static_cast<char>( bit_buffer.to_ulong() );

		return out == FLAG;
	}

	/*
	 * The CRC16 binary table for implementation
	 * according to the CCITT Standard
	 */
	static const unsigned short crc16table[256] = {
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
		0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
		0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
		0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
		0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
		0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
		0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
		0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
		0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
		0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
		0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
		0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
		0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
		0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
		0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
		0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
		0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
		0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
		0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
		0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
		0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
		0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
		0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
		0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
		0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
		0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
		0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
		0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
		0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
		0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
		0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
	};

	/*
	 * Returns the calculated crc value of the buffer with length len
	 */
	unsigned short ax25_sink_b_impl::crc16_ccitt(char *buf, int len) {

		unsigned short crc = 0;
		for(int counter = 0; counter < len; counter++) {
			crc = (crc<<8) ^ crc16table[((crc>>8) ^ *buf++)&0x00FF];
		}

		if(_debug) std::cout << "    FCS: " << packet_crc << "\n    CRC: " << crc << std::endl;

		return crc;
	}

	/*
	 * Function used for differential decoding
	 */
	uint32_t ax25_sink_b_impl::popCount(uint32_t x) {

		uint32_t r = x - ((x >> 1) & 033333333333) - ((x >> 2) & 011111111111);
		return ((r + (r >> 3)) & 030707070707) % 63;
	}

	/*
	 * Determines how many seconds the system has been operational
	 */
	std::time_t ax25_sink_b_impl::uptime() {

		return ((std::time(NULL)) - _startup);
	}

	/*
	 * Returns the first char* conatined in the receive queue
	 * and deletes it from the queue
	 */
	char* ax25_sink_b_impl::receive() {

		// Implement receive functionality with queue
		char* ret = _phy_i.front();
		_phy_i.pop();
		return ret;
	}

	/*
	 * Detects the AX.25 encapsulation frame, checks the CRC value of
	 * the packet and appends the correct packet to the receive queue
	 * or deletes the incorrect packet
	 */
	int ax25_sink_b_impl::work(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
	{

		const char *in = (const char *) input_items[0];
		for(int z = 0; z < noutput_items; z++) {
			switch(state) {

				case POPULATE_BUFFER:

					detect.push(in[z]);
					if(detect.size() == 8) {
						state = DETERMINE_START_FLAG;
					}

					break;


				case DETERMINE_START_FLAG:

					if(check_flag(detect)) {

						while(!detect.empty()) detect.pop();
						detect.push(in[z]);
						state = DETERMINE_END_FLAG;

						break;
					}

					detect.pop();
					detect.push(in[z]);

					break;


				case DETERMINE_END_FLAG:

					detect.push(in[z]);
					if(detect.size() != 8) break;
					if(check_flag(detect)) {

						while(!detect.empty()) detect.pop();
						if(!raw_packet.empty()) state = UNSTUFF_PACKET;
						break;
					}

					raw_packet.push(detect.front());
					detect.pop();

					break;


				case UNSTUFF_PACKET:

					if(_debug) std::cout << "\nUnstuff Packet" << std::endl;
					num = 0;
					i = 0;
					while(!raw_packet.empty()) {

						if(raw_packet.front() == '\x00') num++;

						else num = 0;

						ppacket.push(raw_packet.front());
						raw_packet.pop();

						if(num == 5 && !raw_packet.empty()) {
							raw_packet.pop();
							num = 0;
						}
					}

					state = DIFFERENTIAL_DECODE;
					z--;

					break;

				case DIFFERENTIAL_DECODE:

					if(_debug) std::cout << "Differential Decode" << std::endl;
					last = '\x00';
					while(!ppacket.empty()) {
						decode_packet.push_back((~(ppacket.front() ^ last)  & 1));
						last = ppacket.front();
						ppacket.pop();
					}

					i = 0;
					packet_crc = 0;
					while(i < 16) {
						packet_crc = (decode_packet.back() << (15 - i)) | packet_crc;
						decode_packet.pop_back();
						i++;
					}
					state = DESCRAMBLE;
					z--;
					break;


				case DESCRAMBLE:

					if(_debug) std::cout << "Descramble" << std::endl;
					//newbit = '\x00';
					//shift_register = 0;
					for(i = 0; i < decode_packet.size(); i++) {
						ppacket.push(shift_register & 1);
						newbit = (popCount(shift_register & mask) % 2) ^ (decode_packet[i] & 1);
						shift_register = ((shift_register >> 1) | (newbit << shift_register_length));
					}
					if(_debug) std::cout << "    Length: " << ppacket.size() << std::endl;

					i = 0;
					packet = new char[ppacket.size()];
					num = 0;
					t = '\x00';
					while(!ppacket.empty()) {
						t = (ppacket.front() << (7 - i)) | t;
						ppacket.pop();
						i++;

						if(i == 8) {
							packet[num] = t;
							num++;
							t = '\x00';
							i = 0;
						}
					}
					z--;
					state = CALCULATE_CRC16;

					break;

				case CALCULATE_CRC16:

					if(_debug) std::cout << "Calculating CRC16" << std::endl;
					if(crc16_ccitt(packet, strlen(packet)) == packet_crc) {
						if(_debug) std::cout << "adding to receive queue" << std::endl;
						//_phy_i.push(packet);
						char inq[strlen(packet)];
						memcpy(inq, packet, strlen(packet));
						if(_log) std::cout << "\nReceived Packet\n" << "**********************\n" << "Payload: " << *inq << std::endl;
						_phy_i.push(inq);
						if(_log) {
							_log_file << std::setfill('0')
								  << std::setw(12) << _id_num
								  << " [" << timestamp()
								  << " " << std::setfill('0')
								  << std::setw(10) << uptime()
								  << "] " << inq << "\n";
							_log_file.flush();
						}
						_id_num++;
					}
					delete[] packet;
					z--;
					state = POPULATE_BUFFER;

					break;


				default:

					if(_debug) std::cout << "Default" << std::endl;
					while(!detect.empty()) detect.pop();
					while(!raw_packet.empty()) raw_packet.pop();
					packet = NULL;
					state = POPULATE_BUFFER;

			}
		}

		return noutput_items;
	}

  } /* namespace Interfaces */
} /* namespace gr */

