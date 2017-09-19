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

#ifndef INCLUDED_DPD_FASTRLS_DPD_IMPL_H
#define INCLUDED_DPD_FASTRLS_DPD_IMPL_H

#include <dpd/fastRLS_DPD.h>
#include <armadillo>

using namespace arma;

namespace gr {
  namespace dpd {

    class fastRLS_DPD_impl : public fastRLS_DPD
    {
     private:
      std::vector<int> d_dpd_params;
      // bool d_init;
      long d_ofdm_block_index_received;
      gr_complex d_pa_input;
      bool d_ack_predistorter_vec_updated; 
      int d_sample_index_received;
      const int K_a;
      const int L_a;
      const int K_b;
      const int M_b;
      const int L_b;
      const int M;
      const int M_bar;
      std::ofstream output_file, PA_output_file;
      const int sreg_len = 50;
      gr_complex sreg[50];      
      const gr_complex *ptr_sreg = sreg;

      // Fast-RLS parameter declaration
      float lambda;
      float eta;
      float inv_sqrt_gamma_iMinus1;
      cx_fmat g_vec_iMinus1;
      cx_fmat g_vec_i;
      cx_fmat L_bar_iMinus1;
      cx_fmat w_i;
      cx_fmat w_iMinus1;

      // private functions
      void init_params(float &lambda, float &eta, float &inv_sqrt_gamma_iMinus1, 
		cx_fmat &g_vec_iMinus1, cx_fmat &L_bar_iMinus1, cx_fmat &w_iMinus1);

     public:
      fastRLS_DPD_impl(const std::vector<int> &dpd_params);
      ~fastRLS_DPD_impl();

      void get_PA_input(pmt::pmt_t P);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_FASTRLS_DPD_IMPL_H */

