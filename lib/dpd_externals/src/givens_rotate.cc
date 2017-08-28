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

#include "../include/givens_rotate.h"

void givens_rotate(const cx_fmat & in, cx_fmat & out) 
{
  cx_fmat theta(2, 2, fill::zeros);
  gr_complex alpha; 
  float scale, norm;
  cx_fmat g(2, 2, fill::ones);
    
  // algorithm similar to LAPACK's crotg()
  // except for the application of scale adjustment matrix, g
  float a = abs(in(0, 0));
  float b = abs(in(0, 1));
  if (almost_equal(a, 0.0)) { 
    theta(0,1) = 1.0;
    theta(1,0) = -1.0;

    // make the first element of post-array real
    g(0, 1) = std::exp(minus_1i*std::arg(in(0, 1)));
    g(1, 0) = g(0,1);    
  }
  else {
    scale = a+b;
    norm = scale*sqrt(pow(abs(in(0, 0)/scale), 2)+ pow(abs(in(0, 1)/scale), 2));
    alpha = in(0, 0)/a;
    theta(0,0) = a/norm;
    theta(1,1) = theta(0,0);
    theta(1,0) = alpha*conj(in(0, 1))/norm;
    theta(0,1) = -conj(theta(1,0));

    // make the first element of post-array real
    g(0, 0) = std::exp(minus_1i*std::arg(in(0, 0)));
    g(0, 1) = g(0,0);
    g(1, 0) = g(0,0);
    g(1, 1) = g(0,0);  
  }
  theta = g%theta;
  
  // form post-array
  out = in*theta; 
}


bool almost_equal(float a, float b)
{
  // calculate the difference
  float diff_ab = fabs(a - b);
  
  if (diff_ab <= 100.0*std::numeric_limits<float>::epsilon())
    return true;

  return false;
}
