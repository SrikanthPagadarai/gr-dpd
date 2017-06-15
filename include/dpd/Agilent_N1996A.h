/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_DPD_AGILENT_N1996A_H
#define INCLUDED_DPD_AGILENT_N1996A_H

#include <dpd/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dpd {

    /*!
     * \brief <+description of block+>
     * \ingroup dpd
     *
     */
    class DPD_API Agilent_N1996A : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Agilent_N1996A> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dpd::Agilent_N1996A.
       *
       * To avoid accidental use of raw pointers, dpd::Agilent_N1996A's
       * constructor is in a private implementation
       * class. dpd::Agilent_N1996A::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string& ip_addr, float frequency, float span, float resbw, uint32_t nb_points);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_AGILENT_N1996A_H */

