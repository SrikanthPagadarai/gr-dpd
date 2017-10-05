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
#include "phase_calibrate_impl.h"

namespace gr {
  namespace dpd {

    phase_calibrate::sptr
    phase_calibrate::make(int ref_len)
    {
      return gnuradio::get_initial_sptr
        (new phase_calibrate_impl(ref_len));
    }

    /*
     * The private constructor
     */
    phase_calibrate_impl::phase_calibrate_impl(int ref_len)
      : gr::sync_block("phase_calibrate",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_ref_len(ref_len)
    {
      // don't propagate upstream tags
      set_tag_propagation_policy(TPP_DONT);

      d_reference_acquired = false;
      d_reference_samples.reserve(d_ref_len);
      packet_start = false;
      current_sample_index = 0;

      // Setup input port
      message_port_register_in(pmt::mp("samples"));
      set_msg_handler(pmt::mp("samples"),
      boost::bind(&phase_calibrate_impl::get_reference, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    phase_calibrate_impl::~phase_calibrate_impl()
    {
    }

    void 
    phase_calibrate_impl::get_reference(pmt::pmt_t P) 
    {
      d_reference_acquired = true;
      
      // extract reference samples from the message
      int P_vec_len = pmt::length(P);
      for (int i = 0; i < P_vec_len; i++)	
        d_reference_samples.push_back( pmt::c32vector_ref(P, i) );	     
    }

    int
    phase_calibrate_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      // copy private variables accessed by the asynchronous message handler block
      // bool reference_acquired = d_reference_acquired;
      std::vector<gr_complex> reference_samples = d_reference_samples;

      // get number of samples consumed since the beginning of time by this block
      // from port 0
      const uint64_t nread = this->nitems_read(0);
      for (int item = 0; item < noutput_items; item++) 
      {
        //get tag if the current sample has one
        get_tags_in_range(tags, 0, nread+item, nread+item+1);

        if (!tags.size()) {
          packet_start = false;
          current_sample_index++; 
          // std::cout << "Here" << '\n';
        }
        else {
          packet_start = true;
          current_sample_index = 0;	
        }

        if (!d_reference_acquired)
          out[item] = in[item];
        else
          out[item] = std::polar( std::abs(in[item]), std::arg(reference_samples[current_sample_index]) );
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

