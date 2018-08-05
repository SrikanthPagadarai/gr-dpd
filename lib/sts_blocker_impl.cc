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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "sts_blocker_impl.h"

namespace gr {
  namespace dpd {

    sts_blocker::sptr
    sts_blocker::make()
    {
      return gnuradio::get_initial_sptr
        (new sts_blocker_impl());
    }

    /*
     * The private constructor
     */
    sts_blocker_impl::sts_blocker_impl()
      : gr::block("sts_blocker",
      gr::io_signature::make(1, 1, sizeof(gr_complex)),
      gr::io_signature::make(1, 1, sizeof(gr_complex)))      
    {
      // don't propagate upstream tags
      set_tag_propagation_policy(TPP_DONT);

      d_expected_peaks = 5;
      peaks = 0;
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
      int item = 0;
      while (item < ninput_items) 
      {        	
        if (peaks == d_expected_peaks) 
        {
          items_to_copy = ninput_items-item;
          iptr = (const char *)&(in[item]);                    
          optr = (char *)&(out[0]);
          memcpy(optr, iptr, items_to_copy*sizeof(gr_complex));          

          // consume all regardless of items copied
          consume_each(ninput_items);
          // Tell runtime system how many output items we produced.
          return items_to_copy;
        }
        else 
        {
          // get tag if the current sample has one
          get_tags_in_range(tags, 0, nread+item, nread+item+1);

          // tag indicates correlation peak on the previous sample
          if (tags.size()) 
          {
            peaks++;
            std::cout << "STS-Blocker: Peak #" << peaks << " detected on sample #" << nread+item-1 << std::endl;          

            // upon detecting the expected number of peaks,
            // the one-sample delay needs to be taken into account
            if (peaks == d_expected_peaks) 
              item--;
          }
          item++;
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


