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
#include "predistorter_impl.h"
#include <armadillo>

using std::vector;
using namespace arma;

namespace gr {
  namespace dpd {

    predistorter::sptr
    predistorter::make(int NFFT, int cp_len, int ovx, int num_zero_syms, const std::vector<int> &dpd_params)
    {
      return gnuradio::get_initial_sptr
        (new predistorter_impl(NFFT, cp_len, ovx, num_zero_syms, dpd_params));
    }

    /*
     * The private constructor
     */
    predistorter_impl::predistorter_impl(int NFFT, int cp_len, int ovx, int num_zero_syms, const std::vector<int> &dpd_params)
      : gr::sync_block("predistorter",
              gr::io_signature::make(1, 1, (dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4])*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_NFFT(NFFT), 
              d_cp_len(cp_len), 
              d_ovx(ovx),
              d_num_zero_syms(num_zero_syms),
              d_dpd_params(dpd_params),
              K_a(d_dpd_params[0]), 
              L_a(d_dpd_params[1]),
              K_b(d_dpd_params[2]),
              M_b(d_dpd_params[3]),
              L_b(d_dpd_params[4]),
              M(dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4]),
              M_bar(dpd_params[0] + dpd_params[2]*dpd_params[3])
    {
        d_update_predistorter_vec = true;

        d_predistorter_colvec.set_size(M);
        d_predistorter_colvec.zeros();
        d_predistorter_colvec(0) = gr_complex(1.0, 0.0);

        // don't propagate upstream tags
        set_tag_propagation_policy(TPP_DONT);

        // setup output message port and 
        // send predistorted PA input to the Fast-RLS DPD block
        message_port_register_out(pmt::mp("PA_input"));

        // Setup input port
        message_port_register_in(pmt::mp("taps"));
        set_msg_handler(pmt::mp("taps"),
        boost::bind(&predistorter_impl::set_taps, this, _1));
    }	

    /*
     * Our virtual destructor.
     */
    predistorter_impl::~predistorter_impl()
    {
    }

    void 
    predistorter_impl::set_taps(pmt::pmt_t P) 
    {
        d_update_predistorter_vec = true;

        int P_vec_len = pmt::length(P);

        // extract predistorter weight vector from the message
        for (int i = 0; i < P_vec_len; i++)	
            d_predistorter_colvec(i) = pmt::c32vector_ref(P, i);
        
    }

    int
    predistorter_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>		
      // copy private variables accessed by the asynchronous message handler block
      cx_fcolvec predistorter_colvec = d_predistorter_colvec;
      bool update_predistorter_vec = d_update_predistorter_vec;	
		 
      for (int item = 0; item < noutput_items; item++) 
      {
         // PA input which has been arranged in a GMP vector format
         // for predistortion
         vector<gr_complex> yy(in+item*M, in+item*M+M);
         cx_fmat yy_cx_rowvec(1, M);
         yy_cx_rowvec = conv_to<cx_fmat>::from(yy);
         yy_cx_rowvec = strans(yy_cx_rowvec);

         //get number of samples consumed since the beginning of time by this block from port 0
         const uint64_t nread = this->nitems_read(0);

         // container to hold tags
         std::vector<gr::tag_t> tags;

         //get tag if the current sample has one
         get_tags_in_range(tags, 0, nread+item, nread+item+1);

         // keep track of the current sample index relative to the OFDM block length
         static int current_sample_index = 0;
         // keep track of the OFDM block index from which a certain sample is sent
         static int current_ofdm_block_index = -1*d_num_zero_syms-2;

	
         if (!tags.size()) 
            current_sample_index++; 
         else 
         {
            current_sample_index = 0;	
            current_ofdm_block_index++;
         }
	    
         // keep track of the sample index sent to the Fast-RLS DPD block
         static int sent_sample_index = 0;

         // send the STS signal samples as-is, without any predistortion,
         // for the remaining samples, do predistortion and send the PA input to Fast-RLS DPD
         int nskip_predistortion = (d_NFFT+d_cp_len)*d_ovx*d_num_zero_syms + (d_NFFT+d_cp_len)*d_ovx;
         if (nread < nskip_predistortion)
            out[item] = in[item*M];
         else 
         {	
            gr_complex pa_input = as_scalar( yy_cx_rowvec*predistorter_colvec );
            out[item] = pa_input;

            // std::cout << "current_sample_index: " << current_sample_index << std::endl;
            // std::cout << "sent_sample_index: " << sent_sample_index << std::endl;
            if ( (current_sample_index == sent_sample_index) && update_predistorter_vec ) 
            {
               // std::cout << "true sample index sent: " << nread+item << std::endl;
               // std::cout << "OFDM block index sent: " << current_ofdm_block_index << std::endl;

               // std:: cout << "relative sample index sent: " << sent_sample_index << std::endl;

               // create a pmt tuple
               pmt::pmt_t P = pmt::make_tuple(pmt::from_long(current_ofdm_block_index), pmt::from_complex(out[item]));
               message_port_pub(pmt::mp("PA_input"), P);
               
               sent_sample_index++;
               // std::cout << "sent_sample_index: " << sent_sample_index << std::endl;
               update_predistorter_vec = false;
               d_update_predistorter_vec = false;
            }
         }

      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

