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

#ifndef INCLUDED_DPD_EXTERNALS_GEN_GMPVECTOR_H
#define INCLUDED_DPD_EXTERNALS_GEN_GMPVECTOR_H

#include <armadillo>
#include <vector>
#include <complex>

typedef std::complex<float> gr_complex;

using std::vector;
using namespace arma;

void gen_GMPvector(const gr_complex *const in, int item, 
  int K_a, int L_a, int K_b, int M_b, int L_b, vector<gr_complex> &GMP_vector);

#endif /* INCLUDED_DPD_EXTERNALS_GEN_GMPVECTOR_H */
