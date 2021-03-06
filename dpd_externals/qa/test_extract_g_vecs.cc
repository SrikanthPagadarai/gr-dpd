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
#include <dpd_externals/extract_g_vecs.h>
#include <dpd_externals/almost_equals_zero.h>

using namespace std;
using namespace arma;

mat read_from_file(string file_name, int N) {
  ifstream ins(file_name);  
  string line;
  double value;
  int ii = 0;
  mat out = mat(N, 1);
  
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
  mat in_re, in_im, out1_re, out1_im, out2_re, out2_im;
  string oct_str, suffix;
  string in_re_fn, in_im_fn, out1_re_fn, out1_im_fn, out2_re_fn, out2_im_fn;   
  cx_mat in, out1, out2;

  /* Test 1 */
  K_a = 5;
  L_a = 3;
  K_b = 5;
  L_b = 3; 
  M_b = 3;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = mat(M+M_bar, 1);
  in_im = mat(M+M_bar, 1);
  out1_re = mat(M+M_bar, 1);
  out1_im = mat(M+M_bar, 1);
  out2_re = mat(M, 1);
  out2_im = mat(M, 1);

  suffix = "test1";
  oct_str = "./test_extract_g_vecs.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output1
  out1_re_fn = "out1_re_" + suffix + ".txt"; 
  out1_re = read_from_file(out1_re_fn, M+M_bar);
  
  // read imag part of output1
  out1_im_fn = "out1_im_" + suffix + ".txt"; 
  out1_im = read_from_file(out1_im_fn, M+M_bar);

  // read real part of output2
  out2_re_fn = "out2_re_" + suffix + ".txt"; 
  out2_re = read_from_file(out2_re_fn, M);
  
  // read imag part of output2
  out2_im_fn = "out2_im_" + suffix + ".txt"; 
  out2_im = read_from_file(out2_im_fn, M);

  // call extract_g_vecs() with the same input as octave function call 
  in = cx_mat(in_re, in_im);
  out1 = cx_mat(M+M_bar, 1, fill::zeros);
  out2 = cx_mat(M, 1, fill::zeros);
  extract_g_vecs(in, out1, out2, K_a, L_a, K_b, M_b, L_b, M, M+M_bar);

  // check if extract_g_vecs.m output and extract_g_vecs.cc output are equal
  int test1_pass = 1;
  for (int kk = 0; kk < M+M_bar; kk++) {
    if (     ( !almost_equals_zero( std::abs(out1[kk].real()-out1_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out1[kk].imag()-out1_im[kk]), 3 ) )     ) {
      test1_pass = 0;       
      break;
    }
  }
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out2[kk].real()-out2_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out2[kk].imag()-out2_im[kk]), 3 ) )     ) {
      test1_pass = 0;       
      break;
    }
  }

  if (test1_pass)
    std::cout << "extract_g_vecs(): Test 1 Passed." << std::endl;
  else
    std::cerr << "extract_g_vecs(): Test 1 Failed." << std::endl;

  /* Test 2 */
  K_a = 7;
  L_a = 4;
  K_b = 5;
  L_b = 4; 
  M_b = 3;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = mat(M+M_bar, 1);
  in_im = mat(M+M_bar, 1);
  out1_re = mat(M+M_bar, 1);
  out1_im = mat(M+M_bar, 1);
  out2_re = mat(M, 1);
  out2_im = mat(M, 1);

  suffix = "test2";
  oct_str = "./test_extract_g_vecs.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output1
  out1_re_fn = "out1_re_" + suffix + ".txt"; 
  out1_re = read_from_file(out1_re_fn, M+M_bar);
  
  // read imag part of output1
  out1_im_fn = "out1_im_" + suffix + ".txt"; 
  out1_im = read_from_file(out1_im_fn, M+M_bar);

  // read real part of output2
  out2_re_fn = "out2_re_" + suffix + ".txt"; 
  out2_re = read_from_file(out2_re_fn, M);
  
  // read imag part of output2
  out2_im_fn = "out2_im_" + suffix + ".txt"; 
  out2_im = read_from_file(out2_im_fn, M);

  // call extract_g_vecs() with the same input as octave function call 
  in = cx_mat(in_re, in_im);
  out1 = cx_mat(M+M_bar, 1, fill::zeros);
  out2 = cx_mat(M, 1, fill::zeros);
  extract_g_vecs(in, out1, out2, K_a, L_a, K_b, M_b, L_b, M, M+M_bar);

  // check if extract_g_vecs.m output and extract_g_vecs.cc output are equal
  int test2_pass = 1;
  for (int kk = 0; kk < M+M_bar; kk++) {
    if (     ( !almost_equals_zero( std::abs(out1[kk].real()-out1_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out1[kk].imag()-out1_im[kk]), 3 ) )     ) {
      test2_pass = 0;       
      break;
    }
  }
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out2[kk].real()-out2_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out2[kk].imag()-out2_im[kk]), 3 ) )     ) {
      test2_pass = 0;       
      break;
    }
  }

  if (test2_pass)
    std::cout << "extract_g_vecs(): Test 2 Passed." << std::endl;
  else
    std::cerr << "extract_g_vecs(): Test 2 Failed." << std::endl;

  /* Test 3  */
  K_a = 7;
  L_a = 1;
  K_b = 1;
  L_b = 1; 
  M_b = 1;
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;
  in_re = mat(M+M_bar, 1);
  in_im = mat(M+M_bar, 1);
  out1_re = mat(M+M_bar, 1);
  out1_im = mat(M+M_bar, 1);
  out2_re = mat(M, 1);
  out2_im = mat(M, 1);

  suffix = "test3";
  oct_str = "./test_extract_g_vecs.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re_fn = "in_re_" + suffix + ".txt"; 
  in_re = read_from_file(in_re_fn, M+M_bar);
  
  // read imag part of input
  in_im_fn = "in_im_" + suffix + ".txt"; 
  in_im = read_from_file(in_im_fn, M+M_bar);

  // read real part of output1
  out1_re_fn = "out1_re_" + suffix + ".txt"; 
  out1_re = read_from_file(out1_re_fn, M+M_bar);
  
  // read imag part of output1
  out1_im_fn = "out1_im_" + suffix + ".txt"; 
  out1_im = read_from_file(out1_im_fn, M+M_bar);

  // read real part of output2
  out2_re_fn = "out2_re_" + suffix + ".txt"; 
  out2_re = read_from_file(out2_re_fn, M);
  
  // read imag part of output2
  out2_im_fn = "out2_im_" + suffix + ".txt"; 
  out2_im = read_from_file(out2_im_fn, M);

  // call extract_g_vecs() with the same input as octave function call 
  in = cx_mat(in_re, in_im);
  out1 = cx_mat(M+M_bar, 1, fill::zeros);
  out2 = cx_mat(M, 1, fill::zeros);
  extract_g_vecs(in, out1, out2, K_a, L_a, K_b, M_b, L_b, M, M+M_bar);

  // check if extract_g_vecs.m output and extract_g_vecs.cc output are equal
  int test3_pass = 1;
  for (int kk = 0; kk < M+M_bar; kk++) {
    if (     ( !almost_equals_zero( std::abs(out1[kk].real()-out1_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out1[kk].imag()-out1_im[kk]), 3 ) )     ) {
      test3_pass = 0;       
      break;
    }
  }
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equals_zero( std::abs(out2[kk].real()-out2_re[kk]), 3 ) ) || ( !almost_equals_zero( std::abs(out2[kk].imag()-out2_im[kk]), 3 ) )     ) {
      test3_pass = 0;       
      break;
    }
  }

  if (test3_pass)
    std::cout << "extract_g_vecs(): Test 3 Passed." << std::endl;
  else
    std::cerr << "extract_g_vecs(): Test 3 Failed." << std::endl; 

  return 0;
}
