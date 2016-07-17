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

#ifndef INCLUDED_INTERFACES_AX25_QUEUE_SOURCE_B_IMPL_H
#define INCLUDED_INTERFACES_AX25_QUEUE_SOURCE_B_IMPL_H

#include <Interfaces/ax25_queue_source_b.h>
#include <queue>
#include <string>
#include <fstream>
#include <boost/thread/mutex.hpp>

namespace gr {
  namespace Interfaces {

    class ax25_queue_source_b_impl : public ax25_queue_source_b
    {
     private:
	static const int WAIT_TIME = 1000;
                std::ofstream _log_file;

                // Initialize States
                enum State {
                        BLOCKING = 0,
                        ADDRESS_FIELD = 1,
                        CONTROL_FIELD = 2,
                        INFO_FIELD = 3,
                        PADDING = 4
                } state;

                // Initialize Private Variables
                std::queue<char *> _phy_i;
                char _preamble;
                int _pac_len;
                int _hold;
                char* _packet;
                bool _log;
                double _id_num;
                std::time_t _startup;
                int _samples_per_symbol;
                int _bits_per_symbol;

                // Thread specific variables
                boost::mutex q_mutex;
                gr::thread::condition_variable d_not_empty;

                /*
                 * This function converts an integer to a single byte (char)
                 */
                char itob(int num);
                const std::string timestamp();
                std::time_t uptime();
                void blocking();
                int gcd(int a, int b);
                int lcm(int a, int b);
                int n_padding_bytes();
		void bit_stuff(char* pac);

		// Stuff for CRC Calculations
		unsigned short calculate_crc(char* buf, int len);

     public:
	ax25_queue_source_b_impl(char preamble, bool txlog);
	~ax25_queue_source_b_impl();

	void send(char * packet);
	int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace Interfaces
} // namespace gr

#endif /* INCLUDED_INTERFACES_AX25_QUEUE_SOURCE_B_IMPL_H */

