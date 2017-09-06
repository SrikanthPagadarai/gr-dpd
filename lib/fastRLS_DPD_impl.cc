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
#include "fastRLS_DPD_impl.h"
#include <dpd_externals/gen_GMPvector.h>
#include <dpd_externals/extract_postdistorted_y.h>
#include <dpd_externals/extract_g_vecs.h>
#include <dpd_externals/givens_rotate.h>
#include <dpd_externals/hgivens_rotate.h>
#include <dpd_externals/apply_rotations.h>
#include <armadillo>
#include <fstream>

#define COPY_MEM false  // Do not copy matrices into separate memory
#define FIX_SIZE true // Keep dimensions of matrices constant

using namespace std;
using std::string;
using namespace arma;

namespace gr {
  namespace dpd {

    fastRLS_DPD::sptr
    fastRLS_DPD::make(const std::vector<int> &dpd_params)
    {
      return gnuradio::get_initial_sptr
        (new fastRLS_DPD_impl(dpd_params));
    }

    /*
     * The private constructor
     */
    fastRLS_DPD_impl::fastRLS_DPD_impl(const std::vector<int> &dpd_params)
      : gr::sync_block("fastRLS_DPD",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float))),
	      d_dpd_params(dpd_params),
	      K_a(d_dpd_params[0]), 
	      L_a(d_dpd_params[1]),
	      K_b(d_dpd_params[2]),
       	      M_b(d_dpd_params[3]),
              L_b(d_dpd_params[4]),
              M(dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4]),
              M_bar(dpd_params[0] + dpd_params[2]*dpd_params[3])
    {
      d_init = false;
      d_ack_predistorter_vec_updated = false;
      d_sample_index_received = -1;

      // setup output message port and 
      // send predistorted PA input to the Fast-RLS DPD block
      message_port_register_out(pmt::mp("taps"));

      // Setup Input port
      message_port_register_in(pmt::mp("PA_input"));
      set_msg_handler(pmt::mp("PA_input"),
                      boost::bind(&fastRLS_DPD_impl::get_PA_input, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    fastRLS_DPD_impl::~fastRLS_DPD_impl()
    {
    }

    void 
    fastRLS_DPD_impl::get_PA_input(pmt::pmt_t P) 
    {	
      d_ofdm_block_index_received = pmt::to_long(pmt::tuple_ref(P, 0));
      d_pa_input = pmt::to_complex(pmt::tuple_ref(P, 1));
      d_ack_predistorter_vec_updated = true;

      // keep track of the sample index received by the Fast-RLS DPD block
      d_sample_index_received++;
    }

    void 
    fastRLS_DPD_impl::init_params(float &lambda, float &eta, float &inv_sqrt_gamma_iMinus1, 
	cx_fmat &g_vec_iMinus1, cx_fmat &L_bar_iMinus1, cx_fmat &w_iMinus1)
    {
      //constants
      int k = 10;
      lambda = 1-pow(2, 1-k);
      eta = pow(2, k);

      //inverse of square-root of gamm
      inv_sqrt_gamma_iMinus1 = 1;

      //g vector
      g_vec_iMinus1 = zeros<cx_fmat>(M+M_bar, 1);

      //L-bar matrix
      cx_fmat temp_cx_fmat1(L_a+1, 2, fill::zeros);
      temp_cx_fmat1(0, 0) = gr_complex(sqrt(eta*lambda), 0);
      temp_cx_fmat1(L_a, 1) = sqrt(eta*lambda)*pow(lambda, 0.5*L_a);
      cx_fmat eye_K_a(K_a, K_a, fill::eye);
      cx_fmat L_bar_iMinus1_a = kron(eye_K_a, temp_cx_fmat1);

      cx_fmat temp_cx_fmat2(L_b+1, 2, fill::zeros);
      temp_cx_fmat2(0, 0) = gr_complex(sqrt(eta*lambda), 0);
      temp_cx_fmat2(L_b, 1) = sqrt(eta*lambda)*pow(lambda, 0.5*L_b);
      cx_fmat eye_K_bM_b(K_b*M_b, K_b*M_b, fill::eye);
      cx_fmat L_bar_iMinus1_b = kron(eye_K_bM_b, temp_cx_fmat2);

      L_bar_iMinus1 = zeros<cx_fmat>(K_a*(L_a+1)+K_b*M_b*(L_b+1), (K_a+K_b*M_b)*2);
      L_bar_iMinus1( span(0, K_a*(L_a+1)-1), span(0, K_a*2-1) ) = L_bar_iMinus1_a;
      L_bar_iMinus1( span(K_a*(L_a+1), K_a*(L_a+1)+K_b*M_b*(L_b+1)-1), span(K_a*2, (K_a+K_b*M_b)*2-1) ) = L_bar_iMinus1_b;

      //weight-vector
      w_iMinus1 = zeros<cx_fmat>(M, 1);
      w_iMinus1(0,0) = gr_complex(1.0, 0.0);

      //reset init flag
      d_init = 1;        
    }

    int
    fastRLS_DPD_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      float *out = (float *) output_items[0];

      // Do <+signal processing+>
      // copy private variables accessed by the asynchronous message handler block
      long ofdm_block_index_received = d_ofdm_block_index_received;
      gr_complex pa_input = d_pa_input;
      bool ack_predistorter_vec_updated = d_ack_predistorter_vec_updated;
      int sample_index_received = d_sample_index_received;

      // Fast-RLS parameter declaration
      static float lambda;
      static float eta;
      static float inv_sqrt_gamma_iMinus1;
      static cx_fmat g_vec_iMinus1(M+M_bar, 1);
      static cx_fmat g_vec_i(M, 1);
      static cx_fmat L_bar_iMinus1(M+M_bar, M_bar*2);
      static cx_fmat w_i(M, 1);
      static cx_fmat w_iMinus1(M, 1);

      cx_fmat yy_times_L_bar;
      cx_fmat A_mat(M+M_bar+1, M_bar*2+1, fill::zeros);
      cx_fmat B_mat(M+M_bar+1, M_bar*2+1, fill::zeros);

      // Fast-RLS parameter initialization
      if (!d_init) 
      {
        init_params(lambda, eta, inv_sqrt_gamma_iMinus1, 
                    g_vec_iMinus1, L_bar_iMinus1, w_iMinus1);

        d_init = true;
      }

      static int iteration = 1;
      for (int item = 0; item < noutput_items; item++) 
      {
        // for (int item = history()-1; item < noutput_items+history()-1; item++) {
        // get number of samples consumed since the beginning of time by this block
        // from port 0
        const uint64_t nread = this->nitems_read(0);

        // container to hold tags
        std::vector<gr::tag_t> tags;

        //get tag if the current sample has one
        get_tags_in_range(tags, 0, nread+item, nread+item+1);

        // keep track of the current sample index relative to the OFDM block length
        static int current_sample_index = 0;
        // keep track of the OFDM block index from which a certain sample is sent
        static int current_ofdm_block_index = -1;

        // current_sample_index = ( tags.size() ) ? 0: current_sample_index+1;
        if (!tags.size()) 
          current_sample_index++; 
        else 
        {
          current_sample_index = 0;	
          current_ofdm_block_index++;
        }

        // std::cout << "Inside Fast-RLS DPD work function..." << std::endl;
        static cx_float error = 0.1;
        out[item] = 10.0*log10(pow(abs(error), 2));
        if ( (current_sample_index == sample_index_received) && (current_ofdm_block_index == ofdm_block_index_received) && ack_predistorter_vec_updated ) 
        {
          // std::cout << "pa_input (received by Fast RLS-DPD): " << pa_input << std::endl;
          std::cout << "iteration: " << iteration << std::endl;	    
          iteration++;

          // std::cout << "predistorter_vec_updated: " << predistorter_vec_updated << std::endl;
          std::cout << "true sample index received: " << nread+item << endl;
          std::cout << "OFDM block index applied to: " << current_ofdm_block_index << std::endl;
          std::cout << "relative sample index received: " << sample_index_received << endl;

          int yy_len = M+M_bar;

          // extracting the PA output and arranging into a shift-structured GMP vector
          vector<gr_complex> temp_out1;
          temp_out1.reserve(M_bar);         
          gen_GMPvector(in, item, K_a, L_a+1, K_b, M_b, L_b+1, temp_out1);
          cx_frowvec yy_cx_frowvec(M+M_bar);
          for (int ii = 0; ii < (M+M_bar); ii++)
            yy_cx_frowvec(ii) = temp_out1[ii];

          // yy_times_L_bar = gr_complex(0.01, 0.0)*yy_cx_frowvec*L_bar_iMinus1;
          yy_times_L_bar = yy_cx_frowvec*L_bar_iMinus1;

          // A-matrix
          A_mat.submat(0, 0, 0, 0) = inv_sqrt_gamma_iMinus1;
          A_mat.submat(0, 1, 0, M_bar*2) = yy_times_L_bar;
          A_mat.submat(1, 0, M+M_bar, 0) = g_vec_iMinus1;
          A_mat.submat(1, 1, M+M_bar, M_bar*2) = L_bar_iMinus1;

          // obtain B-matrix by performing Givens and Hyperbolic rotations
          apply_rotations(A_mat, B_mat);
          // if (iteration > 51 && iteration > 61) {
            char numstr[21]; // enough to hold all numbers up to 64-bits
            sprintf(numstr, "%d", iteration);
            std::string file_name1, file_name2;
            std::string prefix1 = "/home/radio1/Documents/gr-dpd/examples/A_mat";
            file_name1 = prefix1+numstr+".csv";
            A_mat.save( file_name1, csv_ascii );

            std::string prefix2 = "/home/radio1/Documents/gr-dpd/examples/B_mat";
            file_name2 = prefix2+numstr+".csv";
            B_mat.save( file_name2, csv_ascii );
          //}
			
          //get time-updates for gamma
          static gr_complex inv_sqrt_gamma_i = B_mat(0, 0);
          static float gamma_i = 1.0/pow(real(inv_sqrt_gamma_i), 2);
          // std::cout << "gamma_i: " << gamma_i << std::endl;
		
          // get time-updates for g-vector
          cx_fmat g = B_mat(span(1, M+M_bar), 0);    
          extract_g_vecs(g, g_vec_iMinus1, g_vec_i, K_a, L_a, K_b, M_b, L_b, M, M+M_bar);    

          // adjust post-distorted PA output dimensions
          cx_fmat y(1, M, fill::zeros);
          extract_postdistorted_y(yy_cx_frowvec, y, K_a, L_a, K_b, M_b, L_b, M);

          //adaptation error
          gr_complex postdistorter_op = as_scalar(y*w_iMinus1);
          error = pa_input - postdistorter_op;	        	

          // correction-factor
          cx_fmat c_factor = (error/real(inv_sqrt_gamma_i))*g_vec_i;	  

          // weight-vector
          w_i = w_iMinus1+c_factor;	    
	
          // prepare-quantities for next iteration
          inv_sqrt_gamma_iMinus1 = inv_sqrt_gamma_i.real();
          L_bar_iMinus1 = cx_float(1.0/sqrt(lambda), 0.0) * B_mat( span(1, M+M_bar), span(1, 2*M_bar) );
          w_iMinus1 = w_i;

          out[item] = 10.0*log10(pow(abs(error), 2));

          // send weight-vector to predistorter block in a message
          vector<gr_complex> taps = conv_to< vector<gr_complex> >::from(w_i);
          pmt::pmt_t P_c32vector_taps = pmt::init_c32vector(M, taps);
          message_port_pub(pmt::mp("taps"), P_c32vector_taps);
 
          ack_predistorter_vec_updated = false;
        } 
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
  } /* namespace dpd */
} /* namespace gr */
