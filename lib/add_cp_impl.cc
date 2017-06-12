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
#include "add_cp_impl.h"

#include <vector>
using std::vector;

namespace gr {
  namespace dpd {

    add_cp::sptr
    add_cp::make(int NFFT, int cp_len)
    {
      return gnuradio::get_initial_sptr
        (new add_cp_impl(NFFT, cp_len));
    }

    /*
     * The private constructor
     */
    add_cp_impl::add_cp_impl(int NFFT, int cp_len)
      : gr::sync_block("add_cp",
              gr::io_signature::make(1, 1, NFFT*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, (NFFT+cp_len)*sizeof(gr_complex))),
	      d_NFFT(NFFT),
              d_cp_len(cp_len)
    {}

    /*
     * Our virtual destructor.
     */
    add_cp_impl::~add_cp_impl()
    {
    }

    int
    add_cp_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	int in_vlen = d_NFFT;
	int out_vlen = (d_NFFT+d_cp_len);
	for (int item = 0; item < noutput_items; item++) {
		//symbol vector
		vector<gr_complex> sym_vec(in+item*in_vlen, in+item*in_vlen+in_vlen);

        	//cyclic-prefix
		// vector<gr_complex>::iterator iter1 = sym_vec.end()-d_cp_len*d_ovx, iter2 = sym_vec.end();
                // check if the following line is doing what is expected
                vector<gr_complex>::iterator iter1 = sym_vec.end()-d_cp_len, iter2 = sym_vec.end();
		vector<gr_complex> cp(iter1, iter2);
	
	        //concatenate cyclic-prefix and symbol vector
		vector<gr_complex> sym_vec_with_cp(cp);
	        sym_vec_with_cp.insert(sym_vec_with_cp.end(), sym_vec.begin(), sym_vec.end());

        
		const char *iptr = (const char *)&(sym_vec_with_cp[0]);
		char *optr = (char *)&(out[item*out_vlen]);
		memcpy(optr, iptr, out_vlen*sizeof(gr_complex));
		//std::copy(sym_vec_with_cp.begin(), sym_vec_with_cp.end(), out+item*out_vlen);
		//for (int i = 0; i < out_vlen; i++) 
		    //out[i] = sym_vec_with_cp[i];
	}


        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */
