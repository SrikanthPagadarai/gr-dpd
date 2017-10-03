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
#include "stream_to_gmp_vector_impl.h"
#include <armadillo>
#include <algorithm>
#include <math.h>
#include <dpd_externals/gen_GMPvector.h>

using std::vector;
using namespace arma;

namespace gr {
  namespace dpd {

    stream_to_gmp_vector::sptr
    stream_to_gmp_vector::make(const std::vector<int> &dpd_params)
    {
      return gnuradio::get_initial_sptr
        (new stream_to_gmp_vector_impl(dpd_params));
    }

    /*
     * The private constructor
     */
    stream_to_gmp_vector_impl::stream_to_gmp_vector_impl(const std::vector<int> &dpd_params)
      : gr::sync_block("stream_to_gmp_vector",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, (1 << (int) ceil(log2(dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4])))*sizeof(gr_complex))),
              d_dpd_params(dpd_params),
              K_a(d_dpd_params[0]),
              L_a(d_dpd_params[1]),
	      K_b(d_dpd_params[2]),
       	      M_b(d_dpd_params[3]),
              L_b(d_dpd_params[4]),
              M(dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4]),
              M_bar(dpd_params[0] + dpd_params[2]*dpd_params[3])
    {
      next_pow2_M = (1 << (int) ceil(log2(M)));
      set_history( std::max(L_a, M_b+L_b) );  
    }

    /*
     * Our virtual destructor.
     */
    stream_to_gmp_vector_impl::~stream_to_gmp_vector_impl()
    {
    }

    int
    stream_to_gmp_vector_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int item = history()-1; item < noutput_items+history()-1; item++) {         
         vector<gr_complex> GMP_vector(next_pow2_M, gr_complex(0.0,0.0));
         gen_GMPvector(in, item, K_a, L_a, K_b, M_b, L_b, GMP_vector);
         
         std::copy(GMP_vector.begin(), GMP_vector.end(), out+(item-history()+1)*(M));
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

