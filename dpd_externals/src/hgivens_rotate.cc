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

#include "../include/hgivens_rotate.h"
#include "../include/almost_equal.h"

void hgivens_rotate(const cx_mat & in, cx_mat & out) 
{
  const static gr_complexd minus_1i(0, -1);

  int flip; 
  gr_complexd a, b, c, d1, d2, x, y, z, conj_a, conj_b, p, q;
  double scale, angle_a;

  x = in(0, 0);
  y = in(0, 1);
  if (almost_equal(abs(x), 0.0, 100.0)) {
    angle_a = std::arg(in(0, 1));
    out = std::exp(minus_1i*angle_a)*in;
  }
  else if (almost_equal(abs(y), 0.0, 100.0)) {
    angle_a = std::arg(in(0, 0));
    out = std::exp(minus_1i*angle_a)*in;
  }
  else {
    b = in(0, 1);
    a = in(0, 0);        
    conj_a = conj(a);
    conj_b = conj(b);        
    if ( abs(in(0, 0)) > abs(in(0, 1)) ) {
      flip = 0;     
      scale = abs(a)/sqrt((abs(a) - abs(b)) * (abs(a) + abs(b)));
      d1 = (conj_a-conj_b)/conj_a;
      p = (a+b)/a;
      q = (conj_a+conj_b)/conj_a;   
    }
    else {
      flip = 1;        
      scale = abs(b)/sqrt((abs(b) - abs(a)) * (abs(a) + abs(b)));
      d1 = (b-a)/b;
      p = (conj_a+conj_b)/conj_b;
      q = (a+b)/b;                
    }

    // Algorithm adapted from H-procedure described in 2.6.4 and 2.A of 
    // Fast Reliable Algorithms for Matrices with Structure - Edited by Sayed and Kailath
    for (int ii = 0; ii < in.n_rows; ii++) {
      if ( almost_equal(abs(in(ii, 0)), 0.0, 100.0) && almost_equal(abs(in(ii, 1)), 0.0, 100.0) ) {
        out(ii, 0) = in(ii, 0);
        out(ii, 1) = in(ii, 1); 
      }
      else {
        x = in(ii, 0);
        y = in(ii, 1);    
        if ( !almost_equal(abs(in(ii, 0)), 0.0, 100.0) ) {
          d2 = (x-y)/x;
          z = d1+d2-d1*d2;

          out(ii, 0) = scale * x * z;
          out(ii, 1) = out(ii, 0) - scale*( p*x - q*y );
        }
        else {
          out(ii, 0) = -scale*y*(q-gr_complexd(1.0, 0.0));
          out(ii, 1) = scale*y;                
        }
      }
    }
    
    if (flip == 0)
        out = std::exp(minus_1i*std::arg(out(0,0)))*out;
    else
        out = std::exp(minus_1i*std::arg(out(0,1)))*out;    
  }
}
