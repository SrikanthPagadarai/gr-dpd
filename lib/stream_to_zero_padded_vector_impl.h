/* -*- c++ -*- */
/* 
 * Copyright 2016
 * Srikanth Pagadarai <srikanth.pagadarai@gmail.com>
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

#ifndef INCLUDED_DPD_STREAM_TO_ZERO_PADDED_VECTOR_IMPL_H
#define INCLUDED_DPD_STREAM_TO_ZERO_PADDED_VECTOR_IMPL_H

#include <dpd/stream_to_zero_padded_vector.h>

namespace gr {
  namespace dpd {

    class stream_to_zero_padded_vector_impl : public stream_to_zero_padded_vector
    {
     private:
      int d_used_sc;
      int d_NFFT;      

     public:
      stream_to_zero_padded_vector_impl(int used_sc, int NFFT);
      ~stream_to_zero_padded_vector_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_STREAM_TO_ZERO_PADDED_VECTOR_IMPL_H */

