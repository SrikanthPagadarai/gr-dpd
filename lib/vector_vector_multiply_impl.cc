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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "vector_vector_multiply_impl.h"
#include <armadillo>
#define COPY_MEM false  // Do not copy vectors into separate memory
#define FIX_SIZE true // Keep dimensions of vectors constant

using namespace arma;
namespace gr {
  namespace dpd {

    vector_vector_multiply::sptr
    vector_vector_multiply::make(const std::vector<gr_complex> &const_vec, int vec_len)
    {
      return gnuradio::get_initial_sptr
        (new vector_vector_multiply_impl(const_vec, vec_len));
    }

    /*
     * The private constructor
     */
    vector_vector_multiply_impl::vector_vector_multiply_impl(const std::vector<gr_complex> &const_vec, int vec_len)
      : gr::sync_block("vector_vector_multiply",
              gr::io_signature::make(1, 1, vec_len*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_vec_len(vec_len)
    {
      arma_const_vec = conv_to< cx_rowvec >::from(const_vec);      
    }

    /*
     * Our virtual destructor.
     */
    vector_vector_multiply_impl::~vector_vector_multiply_impl()
    {
    }

    int
    vector_vector_multiply_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int item = 0; item < noutput_items; item++) 
      {
        cx_fcolvec input_vec( (gr_complex *) input_items[0]+item*d_vec_len, d_vec_len, COPY_MEM, FIX_SIZE );
        out[item] = as_scalar(arma_const_vec*input_vec);               
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

