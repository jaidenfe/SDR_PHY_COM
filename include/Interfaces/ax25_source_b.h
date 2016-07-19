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


#ifndef INCLUDED_INTERFACES_AX25_SOURCE_B_H
#define INCLUDED_INTERFACES_AX25_SOURCE_B_H

#include <Interfaces/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Interfaces {

    /*!
     * \brief <+description of block+>
     * \ingroup Interfaces
     *
     */
    class INTERFACES_API ax25_source_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<ax25_source_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Interfaces::ax25_source_b.
       *
       * To avoid accidental use of raw pointers, Interfaces::ax25_source_b's
       * constructor is in a private implementation
       * class. Interfaces::ax25_source_b::make is the public interface for
       * creating new instances.
       */
      static sptr make(char* src, char* dst);
      virtual void send(char* payload)=0;
    };

  } // namespace Interfaces
} // namespace gr

#endif /* INCLUDED_INTERFACES_AX25_SOURCE_B_H */

