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

#include <string>
#include <gnuradio/io_signature.h>
#include "postdistorter_impl.h"
#include <dpd_externals/gen_GMPvector.h>
#include <dpd_externals/extract_postdistorted_y.h>
#include <dpd_externals/extract_g_vecs.h>
#include <dpd_externals/givens_rotate.h>
#include <dpd_externals/hgivens_rotate.h>
#include <dpd_externals/apply_rotations.h>
#include <dpd_externals/gauss_smooth.h>
#include <armadillo>
#include <fstream>

#define COPY_MEM false  // Do not copy matrices into separate memory
#define FIX_SIZE true // Keep dimensions of matrices constant

using namespace std;
using std::string;
using namespace arma;

namespace gr {
  namespace dpd {

    postdistorter::sptr
    postdistorter::make(const std::vector<int> &dpd_params, int save_log)
    {
      return gnuradio::get_initial_sptr
        (new postdistorter_impl(dpd_params, save_log));
    }

    /*
     * The private constructor
     */
    postdistorter_impl::postdistorter_impl(const std::vector<int> &dpd_params, int save_log)
      : gr::sync_block("postdistorter",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0)),
	      d_dpd_params(dpd_params),
	      K_a(d_dpd_params[0]), 
	      L_a(d_dpd_params[1]),
	      K_b(d_dpd_params[2]),
       	      M_b(d_dpd_params[3]),
              L_b(d_dpd_params[4]),
              M(dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4]),
              M_bar(dpd_params[0] + dpd_params[2]*dpd_params[3]),
              d_save_log(save_log)          
    {
      d_ack_predistorter_updated = false;
      d_received_sample_index = -1;

      // setup output message port  
      message_port_register_out(pmt::mp("taps"));

      // setup input message port
      message_port_register_in(pmt::mp("PA_input"));
      set_msg_handler(pmt::mp("PA_input"), boost::bind(&postdistorter_impl::get_PA_input, this, _1));
      
      relative_sample_index = 0; // keep track of the current sample index relative to the OFDM block length
      current_ofdm_block_index = -1; // keep track of the OFDM block index from which a certain sample is sent       
      iteration = 1;
      error = gr_complex(0.1, 0.0);

      for (int ii = 0; ii < sreg_len; ii++)   
        sreg[ii]=0.0;

      for (int ii=0; ii < 10; ii++)
      {
        sr1[ii]=gr_complexd(0.0,0.0);
        sr2[ii]=gr_complexd(0.0,0.0);
      }

      // constants
      int k = 16;
      lambda = 1-pow(2, 1-k);
      one_over_sqrt_lambda = 1.0/sqrt(lambda);
      eta = pow(2, k);

      g_vec_iMinus1.set_size(M+M_bar, 1);
      g_vec_i.set_size(M, 1);
      L_bar_iMinus1.set_size(M+M_bar, M_bar*2);
      w_i.set_size(M, 1);
      w_iMinus1.set_size(M, 1);

      // inverse of square-root of gamma
      inv_sqrt_gamma_iMinus1 = 1;

      // g vector
      g_vec_iMinus1 = zeros<cx_mat>(M+M_bar, 1);

      // L-bar matrix
      cx_mat temp_cx_mat1(L_a+1, 2, fill::zeros);
      temp_cx_mat1(0, 0) = gr_complexd(sqrt(eta*lambda), 0);
      temp_cx_mat1(L_a, 1) = sqrt(eta*lambda)*pow(lambda, 0.5*L_a);
      cx_mat eye_K_a(K_a, K_a, fill::eye);
      cx_mat L_bar_iMinus1_a = kron(eye_K_a, temp_cx_mat1);
      
      cx_mat temp_cx_mat2(L_b+1, 2, fill::zeros);
      temp_cx_mat2(0, 0) = gr_complexd(sqrt(eta*lambda), 0);
      temp_cx_mat2(L_b, 1) = sqrt(eta*lambda)*pow(lambda, 0.5*L_b);
      cx_mat eye_K_bM_b(K_b*M_b, K_b*M_b, fill::eye);
      cx_mat L_bar_iMinus1_b = kron(eye_K_bM_b, temp_cx_mat2);
      
      L_bar_iMinus1 = zeros<cx_mat>(K_a*(L_a+1)+K_b*M_b*(L_b+1), (K_a+K_b*M_b)*2);
      L_bar_iMinus1( span(0, K_a*(L_a+1)-1), span(0, K_a*2-1) ) = L_bar_iMinus1_a;
      L_bar_iMinus1( span(K_a*(L_a+1), K_a*(L_a+1)+K_b*M_b*(L_b+1)-1), span(K_a*2, (K_a+K_b*M_b)*2-1) ) = L_bar_iMinus1_b;
      
      //weight-vector
      w_iMinus1 = zeros<cx_mat>(M, 1);
      w_iMinus1(0,0) = gr_complexd(1.0, 0.0);

      // A and B matrices
      A_mat.set_size(M+M_bar+1, M_bar*2+1);
      B_mat.set_size(M+M_bar+1, M_bar*2+1); 
      
      yy_cx_fcolvec.set_size(M+M_bar);       
      yy_cx_frowvec.set_size(M+M_bar);       
      y.set_size(1, M);    
      g.set_size(M+M_bar-1, 1);

      if (d_save_log)
      {
        // log-file creation
        time_t rawtime;
        time (&rawtime);
        std::string log_filename = ctime(&rawtime);
        log_filename.erase(std::remove(log_filename.begin(),log_filename.end(),' '),log_filename.end()); // remove spaces
        log_filename.erase(log_filename.size() - 1); // remove return
        log_filename = "/tmp/POSTDISTORTER_LOG_"+log_filename+".txt";
        std::cout << "Writing log messages to " << log_filename << std::endl;
        log_file.open(log_filename.c_str(), std::ios_base::app);
      }      
    }

    /*
     * Our virtual destructor.
     */
    postdistorter_impl::~postdistorter_impl()
    {      
    }

    void 
    postdistorter_impl::get_PA_input(pmt::pmt_t P) 
    {	
      d_ofdm_block_index_received = pmt::to_long(pmt::tuple_ref(P, 0));
      d_pa_input = pmt::to_complex(pmt::tuple_ref(P, 1));
      d_ack_predistorter_updated = true;

      // keep track of the sample index received by the Fast-RLS DPD block
      d_received_sample_index++;
    }
    
    int
    postdistorter_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      
      std::ofstream PA_input_file1, PA_input_file2;
      std::ofstream PA_output_file1, PA_output_file2;
      PA_input_file1.open("/tmp/PA_input_file1.txt", std::ios_base::app);      
      PA_input_file2.open("/tmp/PA_input_file2.txt", std::ios_base::app);      
      PA_output_file1.open("/tmp/PA_output_file1.txt", std::ios_base::app);      
      PA_output_file2.open("/tmp/PA_output_file2.txt", std::ios_base::app);  

      // Do <+signal processing+>
      // copy private variables accessed by the asynchronous message handler block
      ofdm_block_index_received = d_ofdm_block_index_received;
      pa_input = d_pa_input;
      ack_predistorter_updated = d_ack_predistorter_updated;
      received_sample_index = d_received_sample_index;
      
      for (int item = 0; item < noutput_items; item++) 
      {
        // get number of samples consumed since the beginning of time by this block
        // from port 0
        const uint64_t nread = this->nitems_read(0);        

        // get tag if the current sample has one
        get_tags_in_range(tags, 0, nread+item, nread+item+1);

        // relative_sample_index = ( tags.size() ) ? 0: relative_sample_index+1;
        if (!tags.size()) 
          relative_sample_index++; 
        else 
        {
          relative_sample_index = 0;	
          current_ofdm_block_index++;
        }

        if ( (relative_sample_index == received_sample_index) && (current_ofdm_block_index == ofdm_block_index_received) && ack_predistorter_updated ) 
        {
          sr1[0] = pa_input;
          gauss_smooth(sr1, pa_input_smooth);
          sr2[0] = in[item];
          gauss_smooth(sr2, pa_output_smooth);
          std::cout << "Iteration Number: " << iteration << std::endl;	    
          iteration++;



            PA_input_file1 << std::real(pa_input) << '\n' << std::imag(pa_input) << '\n';
            PA_input_file2 << std::real(pa_input_smooth) << '\n' << std::imag(pa_input_smooth) << '\n';
            PA_output_file1 << std::real(in[item]) << '\n' << std::imag(in[item]) << '\n';
            PA_output_file2 << std::real(pa_output_smooth) << '\n' << std::imag(pa_output_smooth) << '\n';




          if (d_save_log)
          {
            log_file << "PA Input (received by Postdistorter): " << pa_input << std::endl;
            log_file << "True sample index received: " << nread+item << endl;
            log_file << "OFDM block index applied to: " << current_ofdm_block_index << std::endl;
            log_file << "Relative sample index received: " << received_sample_index << endl;            
          }

          // extracting the PA output and arranging into a shift-structured GMP vector
          // sreg[49] = pa_output_smooth; 
          sreg[49] = in[item];                   
          gen_GMPvector(ptr_sreg, 49, K_a, L_a+1, K_b, M_b, L_b+1, yy_cx_fcolvec);
          yy_cx_frowvec = yy_cx_fcolvec.st();
          for (int ii = 1; ii < sreg_len; ii++)
            sreg[ii-1] = sreg[ii];          
     
          // A-matrix
          A_mat.submat(0, 0, 0, 0) = inv_sqrt_gamma_iMinus1;
          A_mat.submat(0, 1, 0, M_bar*2) = yy_cx_frowvec*L_bar_iMinus1;
          A_mat.submat(1, 0, M+M_bar, 0) = g_vec_iMinus1;
          A_mat.submat(1, 1, M+M_bar, M_bar*2) = L_bar_iMinus1;

          // obtain B-matrix by performing Givens and Hyperbolic Givens rotations
          apply_rotations(A_mat, B_mat);       
    
          // time-update for 1/sqrt(gamma)
          inv_sqrt_gamma_iMinus1 = real(B_mat(0, 0)); // imag(B_mat(0, 0)) will be ~= 0.0          
          // std::cout << "inv_sqrt_gamma_iMinus1: " << inv_sqrt_gamma_iMinus1 << std::endl;
          	
          // time-update for g-vector
          g = B_mat(span(1, M+M_bar), 0);    
          extract_g_vecs(g, g_vec_iMinus1, g_vec_i, K_a, L_a, K_b, M_b, L_b, M, M+M_bar);              
          
          // adjust post-distorted PA output dimensions
          extract_postdistorted_y(yy_cx_frowvec, y, K_a, L_a, K_b, M_b, L_b, M);
          
          // adaptation error
          error = pa_input - as_scalar(y*w_iMinus1);          

          // update weight-vector
          w_iMinus1 = w_iMinus1+(error/inv_sqrt_gamma_iMinus1)*g_vec_i;	    
	
          // prepare L_bar for next iteration
          L_bar_iMinus1 = gr_complexd(one_over_sqrt_lambda, 0.0) * B_mat( span(1, M+M_bar), span(1, 2*M_bar) );          

          // send weight-vector to predistorter block in a message
          taps = conv_to< vector<gr_complexd> >::from(w_iMinus1);
          pmt::pmt_t P_c32vector_taps = pmt::init_c64vector(M, taps);
          message_port_pub(pmt::mp("taps"), P_c32vector_taps);

          w_iMinus1.save( "/tmp/predistorter_taps.csv", csv_ascii );
 
          ack_predistorter_updated = false;
        } 
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
  } /* namespace dpd */
} /* namespace gr */
