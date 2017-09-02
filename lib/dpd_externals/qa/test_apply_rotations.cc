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

#include <iostream>
#include <fstream>
#include <string> 
#include <armadillo>
#include <dpd_externals/apply_rotations.h>
#include <dpd_externals/almost_equal.h>

using namespace std;
using namespace arma;


fmat read_from_file(string file_name, int N) {
  ifstream ins(file_name);  
  string line;
  float value;
  int ii = 0;
  fmat out = fmat(N, N);
  
  while (std::getline(ins, line)) {
    stringstream ss(line);

    while (ss >> value) {
      out(ii) = value;
      ii++;
    }        
  }  

  return out;
}

int main(void) {
  int N;
  string N_str, oct_str, suffix;
  string in_re_fn, in_im_fn, out_re_fn, out_im_fn;
  fmat in_re, in_im, out_re, out_im;
  cx_fmat in, out;
  
  /* Test 1 */
  N = 50;
  in_re = fmat(N, N);
  in_im = fmat(N, N);
  out_re = fmat(N, N);
  out_im = fmat(N, N);
  N_str = to_string(N);
  suffix = "test1";
  oct_str = "./test_apply_rotations.m " + N_str + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, N);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, N);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, N);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, N);
  
  // call apply_rotations() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, N);
  apply_rotations(in, out);

  // check if apply_rotations.m output and apply_rotations.cc output are equal
  int test1_pass = 1;
  for (int kk = 0; kk < N; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 1000.0 ) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 1000.0 ) )     )
        test1_pass = 0;       
    }
  }

  if (test1_pass)
    std::cout << "apply_rotations(): Test 1 Passed." << std::endl;
  else
    std::cerr << "apply_rotations(): Test 1 Failed." << std::endl;

  /* Test 2 */
  N = 60;
  in_re = fmat(N, N);
  in_im = fmat(N, N);
  out_re = fmat(N, N);
  out_im = fmat(N, N);
  N_str = to_string(N);
  suffix = "test2";
  oct_str = "./test_apply_rotations.m " + N_str + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, N);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, N);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, N);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, N);
  
  // call apply_rotations() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, N);
  apply_rotations(in, out);

  // check if apply_rotations.m output and apply_rotations.cc output are equal
  int test2_pass = 1;
  for (int kk = 0; kk < N; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 1000.0 ) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 1000.0 ) )     )
        test1_pass = 0;       
    }
  }

  if (test2_pass)
    std::cout << "apply_rotations(): Test 2 Passed." << std::endl;
  else
    std::cerr << "apply_rotations(): Test 2 Failed." << std::endl;

  /* Test 3 */
  N = 100;
  in_re = fmat(N, N);
  in_im = fmat(N, N);
  out_re = fmat(N, N);
  out_im = fmat(N, N);
  N_str = to_string(N);
  suffix = "test3";
  oct_str = "./test_apply_rotations.m " + N_str + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, N);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, N);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, N);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, N);
  
  // call apply_rotations() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, N);
  apply_rotations(in, out);

  // check if apply_rotations.m output and apply_rotations.cc output are equal
  int test3_pass = 1;
  for (int kk = 0; kk < N; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 1000.0 ) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 1000.0 ) )     )
        test3_pass = 0;       
    }
  }

  if (test3_pass)
    std::cout << "apply_rotations(): Test 3 Passed." << std::endl;
  else
    std::cerr << "apply_rotations(): Test 3 Failed." << std::endl;

  return 0;
}
