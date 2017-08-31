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

#include "../include/apply_rotations.h"
#include "../include/givens_rotate.h"
#include "../include/hgivens_rotate.h"

void apply_rotations(const cx_fmat & A, cx_fmat & B) 
{
  cx_fmat A_bar = A;
       
  for (int i = 1; i < A_bar.n_cols; i++) {				    
    cx_fmat z(A_bar.n_rows, 2, fill::zeros);
    z.col(0) = A_bar.col(0);
    z.col(1) = A_bar.col(i);

    cx_fmat rotated_z(z.n_rows, 2, fill::zeros);
    switch( i%2 ) {
      case 1:
        // apply Givens rotation to z
	givens_rotate(z, rotated_z);		
        break;

      case 0: 
        // apply Hyperbolic-Givens rotation to z
        hgivens_rotate(z, rotated_z);
        break;
    }
	     
    B.col(0) = rotated_z.col(0);            
    if ( i>1 && i<A_bar.n_cols-1 ) {
      B.cols(1, i-1) = A_bar.cols(1, i-1);
      B.col(i) = rotated_z.col(1);
      B.cols(i+1, B.n_cols-1) = A_bar.cols(i+1, A_bar.n_cols-1);
    }
    else if ( i==1 ) {
      B.col(1) = rotated_z.col(1);
      B.cols(2, B.n_cols-1) = A_bar.cols(2, A_bar.n_cols-1);
    }
    else if ( i==A_bar.n_cols-1 ) {
      B.cols(1, B.n_cols-2) = A_bar.cols(1, A_bar.n_cols-2);
      B.col(B.n_cols-1) = rotated_z.col(1);
    }
    A_bar = B;
  }
  B.print("B:");
}
