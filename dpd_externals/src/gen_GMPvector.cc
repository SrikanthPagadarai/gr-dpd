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

#include "../include/gen_GMPvector.h"

void gen_GMPvector(const gr_complex *const in, int item, 
  int K_a, int L_a, int K_b, int M_b, int L_b, cx_fcolvec &GMP_vector)
{
  /* Signal-and-Aligned Envelope */
  // stacking L_a elements in reverse order
  cx_fcolvec y_vec_arma1(L_a, fill::zeros); 
  for (int ii = L_a-1; ii >= 0; ii--) 
    y_vec_arma1(ii) = in[item-ii]; 
  GMP_vector.rows(0, L_a-1) = y_vec_arma1;  

  //store abs() of y_vec_arma1
  cx_fcolvec abs_y_vec_arma1( size(y_vec_arma1), fill::zeros );
  abs_y_vec_arma1.set_real( abs(y_vec_arma1) );

  cx_fcolvec yy_temp;
  yy_temp = y_vec_arma1%abs_y_vec_arma1;
  GMP_vector.rows(L_a, 2*L_a-1) = yy_temp;
  for (int kk = 2; kk<K_a; kk++) {
    // perform element-wise product using the overloaded % operator
    yy_temp = yy_temp%abs_y_vec_arma1;
  
    GMP_vector.rows(kk*L_a, (kk+1)*L_a-1) = yy_temp;
  }

  /* Signal-and-Delayed Envelope */
  // stacking L_b+M_b elements in reverse order
  cx_fcolvec y_vec_arma23(L_b+M_b, fill::zeros); 
  for (int ii = L_b+M_b-1; ii >= 0; ii--) 
    y_vec_arma23(ii) = in[item-ii]; 

  // L_b signal elements
  cx_fcolvec y_vec_arma2 = y_vec_arma23.rows(0, L_b-1);

  // store abs() of y_vec_arma23
  cx_fcolvec abs_y_vec_arma23( size(y_vec_arma23), fill::zeros );
  abs_y_vec_arma23.set_real( abs(y_vec_arma23) );

  for (int mm = 1; mm < M_b+1; mm++) {
    // stacking L_b delayed signal-envelope elements 
    cx_fcolvec abs_y_vec_arma3 = abs_y_vec_arma23.rows(mm, L_b+mm-1);

    cx_fcolvec yy_temp;
    yy_temp = y_vec_arma2%abs_y_vec_arma3;
    GMP_vector.rows(K_a*L_a+(mm-1)*K_b*L_b, K_a*L_a+(mm-1)*K_b*L_b+L_b-1) = yy_temp;
    for (int kk = 2; kk<K_b+1; kk++) {
      // perform element-wise product using the overloaded % operator
      yy_temp = yy_temp%abs_y_vec_arma3;

      GMP_vector.rows(K_a*L_a+(mm-1)*K_b*L_b+(kk-1)*L_b, K_a*L_a+(mm-1)*K_b*L_b+kk*L_b-1) = yy_temp;
    }
  }
}
