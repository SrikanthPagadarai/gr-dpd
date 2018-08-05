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

#include "../include/extract_g_vecs.h"

void extract_g_vecs(cx_mat &g, cx_mat &g_vec_iMinus1, cx_mat &g_vec_i, int K_a, int L_a, int K_b, int M_b, int L_b, int M, int M_bar) 
{
  cx_mat g_vec_i1( K_a*L_a, 1, fill::zeros);
  cx_mat g_tmp1 = g( span(0, K_a*(L_a+1)-1), 0);
  g_tmp1.reshape(L_a+1, K_a);
  g_tmp1.shed_row(L_a);
  g_vec_i1 = vectorise(g_tmp1, 0);
  g_tmp1.insert_rows(0, 1);
  cx_mat g_vec_iMinus1_a = vectorise(g_tmp1, 0);

  cx_mat g_vec_i2_tmp(K_b*L_b, M_b, fill::zeros);
  cx_mat g_vec_iMinus1_b_tmp(K_b*(L_b+1), M_b, fill::zeros);
  for (int mm = 1; mm < M_b+1; mm++) 
  {
    int index1 = K_a*(L_a+1) + (mm-1)*K_b*(L_b+1);
    int index2 = K_a*(L_a+1) + (mm-1)*K_b*(L_b+1)+K_b*(L_b+1);

    cx_mat g_tmp3 = g( span(index1, index2-1), 0);
    g_tmp3.reshape(L_b+1, K_b);
    g_tmp3.shed_row(L_b);
    g_vec_i2_tmp.col(mm-1) = vectorise(g_tmp3, 0);
    g_tmp3.insert_rows(0, 1);
    g_vec_iMinus1_b_tmp.col(mm-1) = vectorise(g_tmp3, 0);
  }
  cx_mat g_vec_i2 = vectorise(g_vec_i2_tmp, 0);
  cx_mat g_vec_iMinus1_b = vectorise(g_vec_iMinus1_b_tmp, 0);

  g_vec_i.rows(0, K_a*L_a-1) = g_vec_i1;
  g_vec_i.rows(K_a*L_a, M-1) = g_vec_i2;

  g_vec_iMinus1.rows(0, K_a*(L_a+1)-1) = g_vec_iMinus1_a;
  g_vec_iMinus1.rows(K_a*(L_a+1), M_bar-1) = g_vec_iMinus1_b;
}
