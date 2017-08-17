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
#include "polynomial_regressor_vector_impl.h"
#include <armadillo>
#include <algorithm>

using std::vector;
using namespace arma;

namespace gr {
  namespace dpd {

    polynomial_regressor_vector::sptr
    polynomial_regressor_vector::make(int NFFT, int cp_len, int ovx, int num_zero_syms, const std::vector<int> &dpd_params)
    {
      return gnuradio::get_initial_sptr
        (new polynomial_regressor_vector_impl(NFFT, cp_len, ovx, num_zero_syms, dpd_params));
    }

    /*
     * The private constructor
     */
    polynomial_regressor_vector_impl::polynomial_regressor_vector_impl(int NFFT, int cp_len, int ovx, int num_zero_syms, const std::vector<int> &dpd_params)
      : gr::sync_block("polynomial_regressor_vector",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, (dpd_params[0]*dpd_params[1] + dpd_params[2]*dpd_params[3]*dpd_params[4])*sizeof(gr_complex))),
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
      set_history( std::max(L_a, M_b+L_b) );  
    }

    /*
     * Our virtual destructor.
     */
    polynomial_regressor_vector_impl::~polynomial_regressor_vector_impl()
    {
    }

    vector<gr_complex> 
    polynomial_regressor_vector_impl::signal_and_aligned_envelope(const gr_complex *in, const int item) 
    {
      //vector that will contain regressor elements of the memory polynomial
      vector<gr_complex> yy1;
      yy1.reserve(K_a*L_a);

      //grabbing L_a elements in reverse order
      cx_fcolvec y_vec_arma1(L_a, fill::zeros); 
      for (int ii = L_a-1; ii >= 0; ii--) 
         y_vec_arma1(ii) = in[item-ii]; 

      //store abs() of y_vec_arma1
      cx_fcolvec abs_y_vec_arma1( size(y_vec_arma1), fill::zeros );
      abs_y_vec_arma1.set_real( abs(y_vec_arma1) );

      yy1.insert(yy1.end(), y_vec_arma1.begin(), y_vec_arma1.end());
      cx_fcolvec yy_temp;
      yy_temp = y_vec_arma1%abs_y_vec_arma1;
      yy1.insert(yy1.end(), yy_temp.begin(), yy_temp.end());
      for (int kk = 2; kk<K_a; kk++) {
         //perform element-wise product using the overloaded % operator
         yy_temp = yy_temp%abs_y_vec_arma1;
  
         yy1.insert(yy1.end(), yy_temp.begin(), yy_temp.end());
      }

      return yy1;
    }

    vector<gr_complex> 
    polynomial_regressor_vector_impl::signal_and_delayed_envelope(const gr_complex *in, const int item) 
    {
      //vector that will contain regressor elements of the memory polynomial
      vector<gr_complex> yy2;
      yy2.reserve(K_b*M_b*L_b);

      //grabbing L_b+M_b elements in reverse order
      cx_fcolvec y_vec_arma23(L_b+M_b, fill::zeros); 
      for (int ii = L_b+M_b-1; ii >= 0; ii--) 
        y_vec_arma23(ii) = in[item-ii]; 

      // L_b signal elements
      cx_fcolvec y_vec_arma2 = y_vec_arma23.rows(0, L_b-1);

      //store abs() of y_vec_arma23
      cx_fcolvec abs_y_vec_arma23( size(y_vec_arma23), fill::zeros );
      abs_y_vec_arma23.set_real( abs(y_vec_arma23) );

      for (int mm = 1; mm < M_b+1; mm++) {
        //grabbing L_b delayed signal-envelope elements 
        cx_fcolvec abs_y_vec_arma3 = abs_y_vec_arma23.rows(mm, L_b+mm-1);

        cx_fcolvec yy_temp;
        yy_temp = y_vec_arma2%abs_y_vec_arma3;
        yy2.insert(yy2.end(), yy_temp.begin(), yy_temp.end());
        for (int kk = 2; kk<K_b+1; kk++) {
          //perform element-wise product using the overloaded % operator
          yy_temp = yy_temp%abs_y_vec_arma3;

          yy2.insert(yy2.end(), yy_temp.begin(), yy_temp.end());
        }
      }

      return yy2;
    }

    int
    polynomial_regressor_vector_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int item = history()-1; item < noutput_items+history()-1; item++) {
         //vector that will contain regressor elements of the memory polynomial
         vector<gr_complex> yy;
         yy.reserve(M);

         //signal-and-aligned envelope component  
         vector<gr_complex> yy1 = signal_and_aligned_envelope(in, item);
         yy.insert(yy.end(), yy1.begin(), yy1.end());

         //signal-and-delayed-envelope component
         vector<gr_complex> yy2 = signal_and_delayed_envelope(in, item);
         yy.insert(yy.end(), yy2.begin(), yy2.end());

         std::copy(yy.begin(), yy.end(), out+(item-history()+1)*(M));
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

