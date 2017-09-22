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
#include <dpd_externals/almost_equals_zero.h>
#include "gain_phase_calibrate_impl.h"

namespace gr {
  namespace dpd {

    gain_phase_calibrate::sptr
    gain_phase_calibrate::make(int ref_len)
    {
      return gnuradio::get_initial_sptr
        (new gain_phase_calibrate_impl(ref_len));
    }

    /*
     * The private constructor
     */
    gain_phase_calibrate_impl::gain_phase_calibrate_impl(int ref_len)
      : gr::block("gain_phase_calibrate",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_ref_len(ref_len)
    { 
      // don't propagate upstream tags
      set_tag_propagation_policy(TPP_DONT);

      d_reference_acquired = false;
      d_reference_samples.reserve(d_ref_len);

      // Setup input port
      message_port_register_in(pmt::mp("samples"));
      set_msg_handler(pmt::mp("samples"),
      boost::bind(&gain_phase_calibrate_impl::set_reference, this, _1));

      output_file.open("/home/radio1/Documents/gr-dpd/examples/current_cfactor.txt", std::ios_base::app);
    }

    /*
     * Our virtual destructor.
     */
    gain_phase_calibrate_impl::~gain_phase_calibrate_impl()
    {
    }

    void 
    gain_phase_calibrate_impl::set_reference(pmt::pmt_t P) 
    {
      d_reference_acquired = true;

      // extract reference samples from the message
      int P_vec_len = pmt::length(P);
      for (int i = 0; i < P_vec_len; i++)	
        d_reference_samples.push_back( pmt::c32vector_ref(P, i) );	     
    }

    int
    gain_phase_calibrate_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items_,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      gr_complex current_cfactor;
      // copy private variables accessed by the asynchronous message handler block
      bool reference_acquired = d_reference_acquired;
      std::vector<gr_complex> reference_samples = d_reference_samples;

      static bool gain_phase_calibrated;
      	
      // get number of samples consumed since the beginning of time by this block
      // from port 0
      const uint64_t nread = this->nitems_read(0);
      int ninput_items = std::min(ninput_items_[0], noutput_items);
      int item = 0; 
      int nitems_to_skip = 0; 
      while (item < ninput_items) {

        // keep track of the current sample index relative to the OFDM block length
        static int current_sample_index = 0;
        static bool packet_start = false;

        // container to hold tags
        std::vector<gr::tag_t> tags;
        //get tag if the current sample has one
        get_tags_in_range(tags, 0, nread+item, nread+item+1);

        if (!tags.size()) {
          current_sample_index++; 
        }
        else {
          packet_start = true;
          current_sample_index = 0;	
        }

        static gr_complex cfactor;
        static gr_complex previous_cfactor = gr_complex(0.0, 0.0);

	 	
        if (gain_phase_calibrated) {
          out[item-nitems_to_skip] = cfactor*in[item];
          item++;
        }
        else {          
          if ( reference_acquired && packet_start && (current_sample_index < d_ref_len) ) 
          {
            current_cfactor = reference_samples[current_sample_index]/in[item];
            cfactor = ( gr_complex(current_sample_index, 0.0)*previous_cfactor + current_cfactor )/gr_complex(current_sample_index+1, 0.0); 

            if ( !almost_equals_zero(std::real(in[item]), 4) && !almost_equals_zero(std::imag(in[item]), 4) )
              previous_cfactor = cfactor;
            item++;
            nitems_to_skip++;

            // output_file << std::real(cfactor) << '\n' << std::real(in[item]) << '\n';
            output_file << current_sample_index << '\n';
            
            if (current_sample_index == d_ref_len-1) {
              gain_phase_calibrated = true;

              std::cout << std::endl;
              std::cout << "Gain/phase calibration factor applied: " << cfactor << std::endl;
              std::cout << "Gain/phase calibration begin... " << std::endl;
              std::cout << std::endl;             
            }
          }          
        } 
      }

      // consume all regardless of items copied
      consume_each(ninput_items);

      if (gain_phase_calibrated) {
        // Tell runtime system how many output items we produced.
        if (nitems_to_skip > 0) {
          // std::cout << "nitems_to_skip: " << nitems_to_skip << std::endl;
          return (ninput_items-nitems_to_skip);
        }
        else 
          return (ninput_items);
		
      }
      else {
        // std::cout << "gain_phase_calibrated: " << gain_phase_calibrated << std::endl;
        return 0;
      }
    }

  } /* namespace dpd */
} /* namespace gr */

