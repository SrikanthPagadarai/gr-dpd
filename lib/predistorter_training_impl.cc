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
#include "predistorter_training_impl.h"
#include <armadillo>
#include <ctime>
#include <string>
#define COPY_MEM false  // Do not copy vectors into separate memory
#define FIX_SIZE true // Keep dimensions of vectors constant

using std::vector;
using namespace arma;

namespace gr {
  namespace dpd {

    predistorter_training::sptr
    predistorter_training::make(int NFFT, int cp_len, int ovx, int num_zero_syms, int M, int save_log)
    {
      return gnuradio::get_initial_sptr
        (new predistorter_training_impl(NFFT, cp_len, ovx, num_zero_syms, M, save_log));
    }

    /*
     * The private constructor
     */
    predistorter_training_impl::predistorter_training_impl(int NFFT, int cp_len, int ovx, int num_zero_syms, int M, int save_log)
      : gr::sync_block("predistorter_training",
              gr::io_signature::make(1, 1, M*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_NFFT(NFFT), 
              d_cp_len(cp_len), 
              d_ovx(ovx),
              d_num_zero_syms(num_zero_syms), 
              d_M(M),
              d_save_log(save_log)          
    {
      d_update_predistorter_training = true;

      d_predistorter_training_colvec.set_size(d_M);
      d_predistorter_training_colvec.zeros();
      d_predistorter_training_colvec(0) = gr_complex(1.0, 0.0);

      relative_sample_index = 0; 
      sent_sample_index = 0;
      current_ofdm_block_index = -1*d_num_zero_syms-2;     
      nskip = (d_NFFT+d_cp_len)*d_ovx*d_num_zero_syms + (d_NFFT+d_cp_len)*d_ovx*2;    

      // don't propagate upstream tags
      set_tag_propagation_policy(TPP_DONT);

      // setup output message port for 
      // sending predistorted PA input to the postdistorter
      message_port_register_out(pmt::mp("PA_input"));

      // setup input message port
      message_port_register_in(pmt::mp("taps"));
      set_msg_handler(pmt::mp("taps"),
      boost::bind(&predistorter_training_impl::get_taps, this, _1));

      if (d_save_log)
      {
        // log-file creation
        time_t rawtime;
        time (&rawtime);
        std::string log_filename = ctime(&rawtime);
        log_filename.erase(std::remove(log_filename.begin(),log_filename.end(),' '),log_filename.end()); // remove spaces
        log_filename.erase(log_filename.size() - 1); // remove return
        log_filename = "/tmp/PREDISTORTER_LOG_"+log_filename+".txt";
        std::cout << "Writing log messages to " << log_filename << std::endl;
        log_file.open(log_filename.c_str(), std::ios_base::app);
      }
    }	

    /*
     * Our virtual destructor.
     */
    predistorter_training_impl::~predistorter_training_impl()
    {
    }

    void 
    predistorter_training_impl::get_taps(pmt::pmt_t P) 
    {
      d_update_predistorter_training = true;      

      // extract predistorter_training weight vector from the message
      for (int i = 0; i < pmt::length(P); i++)	
        d_predistorter_training_colvec(i) = pmt::c64vector_ref(P, i);        
    }

    int
    predistorter_training_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>		
      // copy private variables accessed by the asynchronous message handler block
      predistorter_training_colvec = d_predistorter_training_colvec;
      update_predistorter_training = d_update_predistorter_training;
      	 
      for (int item = 0; item < noutput_items; item++) 
      {
         // get PA input which has been arranged in a GMP vector format
         // for predistortion
         cx_fmat yy_cx_rowvec( ((gr_complex *) input_items[0])+item*d_M, 1, d_M, COPY_MEM, FIX_SIZE ); 
         
         // get number of samples consumed since the beginning of time by this block from port 0
         nread = this->nitems_read(0);

         // get tag if the current sample has one
         get_tags_in_range(tags, 0, nread+item, nread+item+1);

         if (!tags.size()) 
           relative_sample_index++; 
         else 
         {
           relative_sample_index = 0;	// keep track of the current sample index relative to the OFDM block length         
           current_ofdm_block_index++;  // keep track of the OFDM block index from which a certain sample is sent         
         }       
         
         // send the zeros at the beginning and the STS samples without any predistortion         
         // for the remaining samples, apply predistortion and send the PA input to postdistorter
         if (nread >= nskip)           
         {	
           out[item] = as_scalar( conv_to<cx_fmat>::from(yy_cx_rowvec*predistorter_training_colvec) );            

           if ( (relative_sample_index == sent_sample_index) && update_predistorter_training ) 
           {             
             // create a pmt tuple
             pmt::pmt_t P = pmt::make_tuple(pmt::from_long(current_ofdm_block_index), pmt::from_complex(out[item]));
             message_port_pub(pmt::mp("PA_input"), P);               

             if (d_save_log)
             {
               log_file << "PA Input (sent by Predistorter): " << out[item] << std::endl;
               log_file << "True sample index sent: " << nread+item << std::endl;
               log_file << "OFDM block index sent: " << current_ofdm_block_index << std::endl;
               log_file << "Relative sample index sent: " << sent_sample_index << std::endl;
             }
             sent_sample_index++; // keep track of the sample index sent to the Postdistorter block
             update_predistorter_training = false;
             d_update_predistorter_training = false;              
           }
         }
         else
           out[item] = *(((gr_complex *) input_items[0])+item*d_M); 
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
  } /* namespace dpd */
} /* namespace gr */

