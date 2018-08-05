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

#include "../include/extract_postdistorted_y.h"

void extract_postdistorted_y(cx_fmat &y_in, cx_fmat &y, int K_a, int L_a, int K_b, int M_b, int L_b, int M) 
{
  cx_fmat y_vec_1( 1, K_a*L_a, fill::zeros);
  cx_fmat y_tmp1 = y_in( 0, span(0, K_a*(L_a+1)-1) );

  y_tmp1.reshape(L_a+1, K_a);
  y_tmp1.shed_row(L_a);
  y_vec_1 = vectorise(y_tmp1, 0);

  cx_fmat y_vec_2_tmp(K_b*L_b, M_b, fill::zeros);
  for (int mm = 1; mm < M_b+1; mm++) {
    int index1 = K_a*(L_a+1) + (mm-1)*K_b*(L_b+1);
    int index2 = K_a*(L_a+1) + (mm-1)*K_b*(L_b+1)+K_b*(L_b+1);

    cx_fmat y_tmp3 = y_in( 0, span(index1, index2-1) );
    y_tmp3.reshape(L_b+1, K_b);
    y_tmp3.shed_row(L_b);
    y_vec_2_tmp.col(mm-1) = vectorise(y_tmp3, 0);
  }
  cx_fmat y_vec_2 = vectorise(y_vec_2_tmp, 0);
  
  y.cols(0, K_a*L_a-1) = strans(y_vec_1);
  y.cols(K_a*L_a, M-1) = strans(y_vec_2);
}
