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
#include <dpd_externals/hgivens_rotate.h>
#include <dpd_externals/almost_equal.h>

using namespace std;
using namespace arma;


fmat read_from_file(string file_name, int N) {
  ifstream ins(file_name);  
  string line;
  float value;
  int ii = 0;
  fmat out = fmat(N, 2);
  
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
  int N, sp_case;
  string N_str, sp_case_str, oct_str, suffix;
  string in_re_fn, in_im_fn, out_re_fn, out_im_fn;
  fmat in_re, in_im, out_re, out_im;
  cx_fmat in, out;
  
  /* Test 1 */
  N = 20;
  in_re = fmat(N, 2);
  in_im = fmat(N, 2);
  out_re = fmat(N, 2);
  out_im = fmat(N, 2);
  sp_case = 0;
  N_str = to_string(N);
  sp_case_str = to_string(sp_case);
  suffix = "test1";
  oct_str = "./test_hgivens_rotate.m " + N_str + " " + sp_case_str + " " + suffix;
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
  
  // call hgivens_rotate() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, 2);
  hgivens_rotate(in, out);

  // check if hgivens_rotate.m output and hgivens_rotate.cc output are equal
  int test1_pass = 1;
  for (int kk = 0; kk < 2; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 1000.0) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 1000.0) )     )
        test1_pass = 0;       
    }
  }

  if (test1_pass)
    std::cout << "hgivens_rotate(): Test 1 Passed." << std::endl;
  else
    std::cerr << "hgivens_rotate(): Test 1 Failed." << std::endl;

  /* Test 2 */
  N = 80;
  in_re = fmat(N, 2);
  in_im = fmat(N, 2);
  out_re = fmat(N, 2);
  out_im = fmat(N, 2);
  sp_case = 1;
  N_str = to_string(N);
  sp_case_str = to_string(sp_case);
  suffix = "test2";
  oct_str = "./test_hgivens_rotate.m " + N_str + " " + sp_case_str + " " + suffix;
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
  
  // call hgivens_rotate() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, 2);
  hgivens_rotate(in, out);

  // check if hgivens_rotate.m output and hgivens_rotate.cc output are equal
  int test2_pass = 1;
  for (int kk = 0; kk < 2; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 100.0 ) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 100.0 ) )     ) 
        test2_pass = 0;       
    }
  }

  if (test2_pass)
    std::cout << "hgivens_rotate(): Test 2 Passed." << std::endl; 
  else
    std::cerr << "hgivens_rotate(): Test 2 Failed." << std::endl;

  /* Test 3 */
  N = 100;
  in_re = fmat(N, 2);
  in_im = fmat(N, 2);
  out_re = fmat(N, 2);
  out_im = fmat(N, 2);
  sp_case = 2;
  N_str = to_string(N);
  sp_case_str = to_string(sp_case);
  suffix = "test3";
  oct_str = "./test_hgivens_rotate.m " + N_str + " " + sp_case_str + " " + suffix;
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
  
  // call hgivens_rotate() with the same input as octave function call 
  in = cx_fmat(in_re, in_im);
  out = cx_fmat(N, 2);
  hgivens_rotate(in, out);

  // check if hgivens_rotate.m output and hgivens_rotate.cc output are equal
  int test3_pass = 1;
  for (int kk = 0; kk < 2; kk++) {
    for (int jj = 0; jj < N; jj++) {
      if (     ( !almost_equal( out(jj, kk).real(), out_re(jj, kk), 100.0 ) ) || ( !almost_equal( out(jj, kk).imag(), out_im(jj, kk), 100.0 ) )     ) 
        test3_pass = 0;       
    }
  }

  if (test3_pass)
    std::cout << "hgivens_rotate(): Test 3 Passed." << std::endl; 
  else
    std::cerr << "hgivens_rotate(): Test 3 Failed." << std::endl;

  return 0;
}
