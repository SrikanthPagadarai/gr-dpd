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


#ifndef INCLUDED_DPD_ADD_CP_H
#define INCLUDED_DPD_ADD_CP_H

#include <dpd/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dpd {

    /*!
     * \brief <+description of block+>
     * \ingroup dpd
     *
     */
    class DPD_API add_cp : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<add_cp> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dpd::add_cp.
       *
       * To avoid accidental use of raw pointers, dpd::add_cp's
       * constructor is in a private implementation
       * class. dpd::add_cp::make is the public interface for
       * creating new instances.
       */
      static sptr make(int NFFT, int cp_len);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_ADD_CP_H */

