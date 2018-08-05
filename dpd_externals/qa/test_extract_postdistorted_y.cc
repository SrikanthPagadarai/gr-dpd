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

#include <iostream>
#include <fstream>
#include <string> 
#include <armadillo>
#include <dpd_externals/extract_postdistorted_y.h>
#include <dpd_externals/almost_equals_zero.h>

using namespace std;
using namespace arma;

frowvec read_from_file(string file_name, int N) {
  ifstream ins(file_name);  
  string line;
  double value;
  int ii = 0;
  frowvec out = frowvec(N);
  
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
  int K_a, L_a, K_b, L_b, M_b, M, M_bar;
  frowvec in_re, in_im, out_re, out_im;
  string oct_str, suffix;
  string in_re_fn, in_im_fn, out_re_fn, out_im_fn;   
  cx_frowvec in;
  cx_fmat out;

  /* Test 1 */
  K_a = 5;
  L_a = 3;
  K_b = 5;
  L_b = 3; 
  M_b = 3;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = frowvec(M+M_bar);
  in_im = frowvec(M+M_bar);
  out_re = frowvec(M);
  out_im = frowvec(M);

  suffix = "test1";
  oct_str = "./test_extract_postdistorted_y.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, M);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, M);

  // call extract_postdistorted_y() with the same input as octave function call 
  in = cx_frowvec(in_re, in_im);
  out = cx_fmat(1, M, fill::zeros);
  extract_postdistorted_y(in, out, K_a, L_a, K_b, M_b, L_b, M);

  // check if extract_postdistorted_y.m output and extract_postdistorted_y.cc output are equal
  int test1_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out[kk].real()-out_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out[kk].imag()-out_im[kk]), 3 ) )     ) {
      test1_pass = 0;       
      break;
    }
  }

  if (test1_pass)
    std::cout << "extract_postdistorted_y(): Test 1 Passed." << std::endl;
  else
    std::cerr << "extract_postdistorted_y(): Test 1 Failed." << std::endl;

  /* Test 2 */
  K_a = 7;
  L_a = 4;
  K_b = 5;
  L_b = 4; 
  M_b = 3;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = frowvec(M+M_bar);
  in_im = frowvec(M+M_bar);
  out_re = frowvec(M);
  out_im = frowvec(M);

  suffix = "test2";
  oct_str = "./test_extract_postdistorted_y.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, M);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, M);

  // call extract_postdistorted_y() with the same input as octave function call 
  in = cx_frowvec(in_re, in_im);
  out = cx_fmat(1, M, fill::zeros);
  extract_postdistorted_y(in, out, K_a, L_a, K_b, M_b, L_b, M);

  // check if extract_postdistorted_y.m output and extract_postdistorted_y.cc output are equal
  int test2_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out[kk].real()-out_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out[kk].imag()-out_im[kk]), 3 ) )     ) {
      test2_pass = 0;       
      break;
    }
  }

  if (test2_pass)
    std::cout << "extract_postdistorted_y(): Test 2 Passed." << std::endl;
  else
    std::cerr << "extract_postdistorted_y(): Test 2 Failed." << std::endl;

  /* Test 3 */
  K_a = 7;
  L_a = 1;
  K_b = 1;
  L_b = 1; 
  M_b = 1;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = frowvec(M+M_bar);
  in_im = frowvec(M+M_bar);
  out_re = frowvec(M);
  out_im = frowvec(M);

  suffix = "test3";
  oct_str = "./test_extract_postdistorted_y.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output
  out_re_fn = "out_re_" + suffix + ".txt"; 
  out_re = read_from_file(out_re_fn, M);
  
  // read imag part of output
  out_im_fn = "out_im_" + suffix + ".txt"; 
  out_im = read_from_file(out_im_fn, M);

  // call extract_postdistorted_y() with the same input as octave function call 
  in = cx_frowvec(in_re, in_im);
  out = cx_fmat(1, M, fill::zeros);
  extract_postdistorted_y(in, out, K_a, L_a, K_b, M_b, L_b, M);

  // check if extract_postdistorted_y.m output and extract_postdistorted_y.cc output are equal
  int test3_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out[kk].real()-out_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out[kk].imag()-out_im[kk]), 3 ) )     ) {
      test3_pass = 0;       
      break;
    }
  }

  if (test3_pass)
    std::cout << "extract_postdistorted_y(): Test 3 Passed." << std::endl;
  else
    std::cerr << "extract_postdistorted_y(): Test 3 Failed." << std::endl;

  return 0;
}
