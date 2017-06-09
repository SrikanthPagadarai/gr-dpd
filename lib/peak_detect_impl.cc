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
#include "peak_detect_impl.h"

namespace gr {
  namespace dpd {

    peak_detect::sptr
    peak_detect::make(int NFFT, int ovx, int peak_cond)
    {
      return gnuradio::get_initial_sptr
        (new peak_detect_impl(NFFT, ovx, peak_cond));
    }

    /*
     * The private constructor
     */
    peak_detect_impl::peak_detect_impl(int NFFT, int ovx, int peak_cond)
    : gr::sync_block("peak_detect",
    gr::io_signature::make(1, 2, sizeof(gr_complex)),
    gr::io_signature::make(1, 1, sizeof(gr_complex))),
    d_NFFT(NFFT),
    d_ovx(ovx),
    d_peak_cond(peak_cond)
    {
		// don't propagate upstream tags
		set_tag_propagation_policy(TPP_DONT);
    }


    /*
     * Our virtual destructor.
     */
    peak_detect_impl::~peak_detect_impl()
    {
    }

    int
    peak_detect_impl::work(int noutput_items,
    	gr_vector_const_void_star &input_items,
    	gr_vector_void_star &output_items)
    {
		const gr_complex *in_corr = (const gr_complex *) input_items[0];
        const gr_complex *in = (const gr_complex *) input_items[1];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
		for (int i = 0; i < noutput_items; i++) 
		{
    	    out[i] = in[i];
 
	    	// std::cout << "std::real(in_corr[i]): " << std::real(in_corr[i]) << std::endl;
	    	static int count = 0;
   	    	if( (std::real(in_corr[i]) > d_peak_cond ) && (count < 4) )
	    	{ 
				std::cout << "std::real(in_corr[i]): " << std::real(in_corr[i]) << std::endl;
	        	GR_LOG_DEBUG(d_logger, boost::format("Detected peak on sample %1%")%(nitems_written(0)+i));
 		
				count++;

				tag_t tag;
        		tag.offset = nitems_written(0)+i;
				tag.key = pmt::mp("STS found");
				tag.value = pmt::from_long(count);
				add_item_tag(0, tag);

				// std::cout << "count: " << count << std::endl;
 	
  	    	} 
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

