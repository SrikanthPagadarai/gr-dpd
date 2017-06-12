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
#include "stream_to_zero_padded_vector_impl.h"

#include <vector>
using std::vector;

namespace gr {
  namespace dpd {

    stream_to_zero_padded_vector::sptr
    stream_to_zero_padded_vector::make(int used_sc, int NFFT)
    {
      return gnuradio::get_initial_sptr
        (new stream_to_zero_padded_vector_impl(used_sc, NFFT));
    }

    /*
     * The private constructor
     */
    stream_to_zero_padded_vector_impl::stream_to_zero_padded_vector_impl(int used_sc, int NFFT)
      : gr::sync_decimator("stream_to_zero_padded_vector",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, NFFT*sizeof(gr_complex)), used_sc),
	      d_used_sc(used_sc),
	      d_NFFT(NFFT)
    {}

    /*
     * Our virtual destructor.
     */
    stream_to_zero_padded_vector_impl::~stream_to_zero_padded_vector_impl()
    {
    }

    int
    stream_to_zero_padded_vector_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	int out_vlen = d_NFFT;
	int num_zeros = out_vlen-d_used_sc-1;

        for (int item = 0; item < noutput_items; item++) {
	        //generate a vector of zeros
        	vector<gr_complex> g_vecz(num_zeros, gr_complex(0,0));

	        //collect input into two vectors
        	vector<gr_complex> g_vec1(in+item*d_used_sc, in+item*d_used_sc+d_used_sc/2);
	        vector<gr_complex> g_vec2(in+item*d_used_sc+d_used_sc/2, in+item*d_used_sc+d_used_sc);

		//form the overall output vector
	        vector<gr_complex> g_vec_zp;
        	g_vec_zp.reserve(out_vlen);

	        //dc component
        	g_vec_zp.push_back(gr_complex(0,0));
	        g_vec_zp.insert(g_vec_zp.end(), g_vec1.begin(), g_vec1.end());
        	g_vec_zp.insert(g_vec_zp.end(), g_vecz.begin(), g_vecz.end());
	        g_vec_zp.insert(g_vec_zp.end(), g_vec2.begin(), g_vec2.end());

		/*
		for (int i = 0; i < out_vlen; i++) 
			out[item*out_vlen+i] = g_vec_zp[i];
		*/
		//std::copy(g_vec_zp.begin(), g_vec_zp.end(), out+item*out_vlen);
		const char *iptr = (const char *)&(g_vec_zp[0]);
		char *optr = (char *)&(out[item*out_vlen]);
		memcpy(optr, iptr, out_vlen*sizeof(gr_complex));
	
	}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */
