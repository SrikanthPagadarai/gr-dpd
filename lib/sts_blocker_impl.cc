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
#include "sts_blocker_impl.h"

namespace gr {
  namespace dpd {

    sts_blocker::sptr
    sts_blocker::make(int cp_len)
    {
      return gnuradio::get_initial_sptr
        (new sts_blocker_impl(cp_len));
    }

    /*
     * The private constructor
     */
    sts_blocker_impl::sts_blocker_impl(int cp_len)
      : gr::block("sts_blocker",
      gr::io_signature::make(1, 1, sizeof(gr_complex)),
      gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_cp_len(cp_len)
    {
        // don't propagate upstream tags
        set_tag_propagation_policy(TPP_DONT);

        if (d_cp_len == 0)
           d_expected_peaks = 4;
        else
           d_expected_peaks = 5;

    }

    /*
     * Our virtual destructor.
     */
    sts_blocker_impl::~sts_blocker_impl()
    {
    }


    int
    sts_blocker_impl::general_work (int noutput_items,
            gr_vector_int &ninput_items_,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>

      // get number of samples consumed since the beginning of time by this block
      // from port 0
      const uint64_t nread = this->nitems_read(0);

      int ninput_items = std::min(ninput_items_[0], noutput_items);
      static int peaks = 0;

      // container to hold tags
      std::vector<gr::tag_t> tags;

      int item = 0;
      static int count = 0;	
      while (item < ninput_items) 
      {

         //get tag if the current sample has one
         get_tags_in_range(tags, 0, nread+item, nread+item+1);

         if (tags.size()) 
         {
            peaks++;
            std::cout << "peaks detected at STS-Blocker: " << peaks << std::endl;

            for (int k = 0; k<tags.size(); k++) 
	        std::cout<<"Key: "<<pmt::symbol_to_string(tags[k].key)<<std::endl;

            if (peaks == d_expected_peaks) 
            {
                std::cout << "peaks: " << peaks << std::endl;
                item++;
            }
         }

		
         if (peaks < d_expected_peaks) 
         {
              item++;
         }
         else 
         {
            if (count == 0) 
            {
               std::cout << "nread+item: " << nread+item << std::endl;
               count++;
            }

            int n_to_copy = ninput_items - item;

            const char *iptr = (const char *)&(in[item]);
            char *optr = (char *)&(out[0]);
            memcpy(optr, iptr, n_to_copy*sizeof(gr_complex));

            // consume all regardless of items copied
            consume_each(ninput_items);
            // Tell runtime system how many output items we produced.
            return n_to_copy;
          }

      }


      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (ninput_items);

      // Tell runtime system how many output items we produced.
      return 0;
    }

  } /* namespace dpd */
} /* namespace gr */


