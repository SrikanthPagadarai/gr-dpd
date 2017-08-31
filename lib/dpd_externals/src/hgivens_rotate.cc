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

void hgivens_rotate(const cx_fmat & in, cx_fmat & out) 
{
  const static gr_complex minus_1i(0, -1);

  int flip; 
  gr_complex a, b, c, d1, d2, x, y, z, conj_a, conj_b, p, q;
  float abs_a, abs_b, scale, angle_a;

  x = in(0, 0);
  y = in(0, 1);
  if (almost_equal(abs(x), 0.0, 100.0)) {
    angle_a = std::arg(in(0, 1));
    out = std::exp(minus_1i*angle_a)*fliplr(in);
  }
  else if (almost_equal(abs(y), 0.0, 100.0)) {
    angle_a = std::arg(in(0, 0));
    out = std::exp(minus_1i*angle_a)*in;
  }
  else {
    if (abs(x) < abs(y)) {
      flip = 1; 
      b = x;
      a = y; 
    }
    else {
      flip = 0; 
      b = y;
      a = x;
    } 
    conj_a = conj(a);
    conj_b = conj(b);
    p = gr_complex(1.0, 0.0) + b/a;
    q = gr_complex(1.0, 0.0) + conj_b/conj_a;
    abs_a = abs(a);
    abs_b = abs(b);
    angle_a = std::arg(a);
    scale = abs_a/sqrt( (abs_a - abs_b)*(abs_a + abs_b) );

    // Algorithm adapted from H-procedure described in 2.6.4 and 2.A of 
    // Fast Reliable Algorithms for Matrices with Structure - Edited by Sayed and Kailath
    for (int ii = 0; ii < in.n_rows; ii++) {
      if (flip) {
        x = in(ii, 1);
        y = in(ii, 0);
      }
      else {
        x = in(ii, 0);
        y = in(ii, 1);
      }
      c = conj_b*y/(conj_a*x);

      if (abs(c) < 0.5)
        z = gr_complex(1.0, 0.0) - c;
      else {
        d1 = (conj_a-conj_b)/conj_a;
        d2 = (x-y)/x;
        z = d1+d2-d1*d2;
      }      

      out(ii, 0) = std::exp(minus_1i*angle_a)*scale*x*z;
      out(ii, 1) = out(ii, 0) - std::exp(minus_1i*angle_a)*scale*(p*x - q*y);
    }
  }
}
