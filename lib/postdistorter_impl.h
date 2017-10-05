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

#ifndef INCLUDED_DPD_postdistorter_IMPL_H
#define INCLUDED_DPD_postdistorter_IMPL_H

#include <dpd/postdistorter.h>
#include <armadillo>

using namespace arma;

namespace gr {
  namespace dpd {

    class postdistorter_impl : public postdistorter
    {
     private:
      std::vector<int> d_dpd_params;
      long d_ofdm_block_index_received, ofdm_block_index_received;
      gr_complexd d_pa_input, pa_input, error;
      gr_complexd pa_input_smooth, pa_output_smooth;
      gr_complexd sr1[10], sr2[10];
      bool d_ack_predistorter_updated, ack_predistorter_updated; 
      int d_sample_index_received, sample_index_received;
      const int K_a;
      const int L_a;
      const int K_b;
      const int M_b;
      const int L_b;
      const int M;
      const int M_bar;
      const int sreg_len = 50;
      gr_complex sreg[50];      
      const gr_complex *ptr_sreg = sreg;
      std::vector<gr::tag_t> tags; // container to hold tags 
      std::vector<gr_complexd> taps;
      int current_sample_index, current_ofdm_block_index;       
      std::ofstream log_file; 
      int d_save_log, iteration;

      // Fast-RLS parameter declaration
      double lambda, one_over_sqrt_lambda, eta, inv_sqrt_gamma_iMinus1;
      cx_mat g_vec_iMinus1, g_vec_i, L_bar_iMinus1, w_i, w_iMinus1, A_mat, B_mat, g;
      cx_fcolvec yy_cx_fcolvec;
      cx_frowvec yy_cx_frowvec;
      cx_fmat y;

      // private functions
      void init_params(double &lambda, double &eta, double &inv_sqrt_gamma_iMinus1, 
		cx_mat &g_vec_iMinus1, cx_mat &L_bar_iMinus1, cx_mat &w_iMinus1);

     public:
      postdistorter_impl(const std::vector<int> &dpd_params, int save_log);
      ~postdistorter_impl();

      void get_PA_input(pmt::pmt_t P);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_postdistorter_IMPL_H */

