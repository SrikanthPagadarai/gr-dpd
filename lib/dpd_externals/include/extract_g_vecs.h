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

#ifndef INCLUDED_DPD_EXTERNALS_EXTRACT_G_VECS_H
#define INCLUDED_DPD_EXTERNALS_EXTRACT_G_VECS_H

#include <armadillo>

using namespace arma;

void extract_g_vecs(cx_fmat &g, cx_fmat &g_vec_iMinus1, cx_fmat &g_vec_i, int K_a, int L_a, int K_b, int M_b, int L_b, int M, int M_bar); 

#endif /*INCLUDED_DPD_EXTERNALS_EXTRACT_G_VECS_H*/
