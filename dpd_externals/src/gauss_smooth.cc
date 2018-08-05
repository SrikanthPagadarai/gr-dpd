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

#include "../include/gauss_smooth.h"

void gauss_smooth(gr_complexd *sr, gr_complexd &out)
{  
  // Gaussian smoothing
  out = gr_complexd(0.0, 0.0);
  for (int ii = 0; ii < sr_len; ii++)
    out += sr[ii]*w[ii];
   
  // shift the register contents
  for (int jj = sr_len-1; jj > 0; jj--)
    sr[jj] = sr[jj-1];
}
