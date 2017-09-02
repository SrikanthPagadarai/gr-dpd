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
#include <cstdlib> 
#include <fstream>
#include <vector>
#include <string> 
#include <dpd_externals/gen_GMPvector.h>
#include <dpd_externals/almost_equal.h>

using namespace std;

void read_from_file(string file_name, int N, float *out) {
  ifstream ins(file_name);  
  string line;
  float value;
  int ii = 0;
  
  while (std::getline(ins, line)) {
    stringstream ss(line);

    while (ss >> value) {
      out[ii] = value;
      ii++;
    }        
  }  
}

int main(void) {
  int K_a, L_a, K_b, L_b, M_b, M, M_bar, history, item, len = 10000;
  float *in_re, *in_im, *out_re, *out_im;
  string oct_str, suffix;
  string in_re_fn, in_im_fn, out_re_fn, out_im_fn;     

  /* Test 1: Post-Distorter */
  K_a = 5;
  L_a = 3;
  K_b = 5;
  L_b = 3; 
  M_b = 3;
  history = std::max(L_a, M_b+L_b);
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;

  suffix = "test1";
  item = 20; // item needs to be >= history
  oct_str = "./test_gen_GMPvector.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + to_string(item) + " " + to_string(len) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re = (float *)malloc((len+history)*sizeof(float));
  in_re_fn = "in_re_" + suffix + ".txt"; 
  read_from_file(in_re_fn, len+history, in_re);
  
  // read imag part of input
  in_im = (float *)malloc((len+history)*sizeof(float));
  in_im_fn = "in_im_" + suffix + ".txt"; 
  read_from_file(in_im_fn, len+history, in_im);

  // read real part of output
  out_re = (float *)malloc((M+M_bar)*sizeof(float));
  out_re_fn = "out_re_" + suffix + ".txt"; 
  read_from_file(out_re_fn, M+M_bar, out_re);
  
  // read imag part of output
  out_im = (float *)malloc((M+M_bar)*sizeof(float));
  out_im_fn = "out_im_" + suffix + ".txt"; 
  read_from_file(out_im_fn, M+M_bar, out_im);

  // call gen_GMPvector() with the same input as octave function call 
  gr_complex in1[len+history];
  for (int ii = 0; ii < len+history; ii++)
    in1[ii] = gr_complex(in_re[ii], in_im[ii]);   
  vector<gr_complex> out1;
  out1.reserve(M+M_bar);         
  gen_GMPvector(in1, item-1, K_a, L_a+1, K_b, M_b, L_b+1, out1);

  /*
  for (int ii = 0; ii < M+M_bar; ii++)
    std::cout << "out_re[ii]: " << out1[ii].real() << "      out_im[ii]: " << out1[ii].imag() << std::endl;   
  */

  // check if gen_GMPvector.m output and gen_GMPvector.cc output are equal
  int test1_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equal( out1[kk].real(), out_re[kk], 1000.0 ) ) || ( !almost_equal( out1[kk].imag(), out_im[kk], 1000.0 ) )     ) {
      test1_pass = 0;       
      break;
    }
  }

  if (test1_pass)
    std::cout << "gen_GMPvector(): Test 1 Passed." << std::endl;
  else
    std::cerr << "gen_GMPvector(): Test 1 Failed." << std::endl;

  /* Test 2: Pre-Distorter */
  K_a = 5;
  L_a = 3;
  K_b = 5;
  L_b = 3; 
  M_b = 3;
  history = std::max(L_a, M_b+L_b);
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;

  suffix = "test2";
  item = 20; // item needs to be >= history
  oct_str = "./test_gen_GMPvector.m " + to_string(K_a) + " " + to_string(L_a-1) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b-1) + " " + to_string(item) + " " + to_string(len) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re = (float *)realloc(in_re, (len+history)*sizeof(float));
  in_re_fn = "in_re_" + suffix + ".txt"; 
  read_from_file(in_re_fn, len+history, in_re);
  
  // read imag part of input
  in_im = (float *)realloc(in_im, (len+history)*sizeof(float));
  in_im_fn = "in_im_" + suffix + ".txt"; 
  read_from_file(in_im_fn, len+history, in_im);

  // read real part of output
  out_re = (float *)realloc(out_re, M*sizeof(float));
  out_re_fn = "out_re_" + suffix + ".txt"; 
  read_from_file(out_re_fn, M, out_re);
  
  // read imag part of output
  out_im = (float *)realloc(out_im, M*sizeof(float));
  out_im_fn = "out_im_" + suffix + ".txt"; 
  read_from_file(out_im_fn, M, out_im);

  // call gen_GMPvector() with the same input as octave function call 
  gr_complex in2[len+history];
  for (int ii = 0; ii < len+history; ii++)
    in2[ii] = gr_complex(in_re[ii], in_im[ii]);   
  vector<gr_complex> out2;
  out2.reserve(M);         
  gen_GMPvector(in2, item-1, K_a, L_a, K_b, M_b, L_b, out2);

  /*
  for (int ii = 0; ii < M; ii++)
    std::cout << "out_re[ii]: " << out2[ii].real() << "      out_im[ii]: " << out2[ii].imag() << std::endl;   
  */

  // check if gen_GMPvector.m output and gen_GMPvector.cc output are equal
  int test2_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equal( out2[kk].real(), out_re[kk], 1000.0 ) ) || ( !almost_equal( out2[kk].imag(), out_im[kk], 1000.0 ) )     ) {
      test2_pass = 0;       
      break;
    }
  }

  if (test2_pass)
    std::cout << "gen_GMPvector(): Test 2 Passed." << std::endl;
  else
    std::cerr << "gen_GMPvector(): Test 2 Failed." << std::endl;

  /* Test 3: Post-Distorter */
  K_a = 5;
  L_a = 3;
  K_b = 7;
  L_b = 1; 
  M_b = 1;
  history = std::max(L_a, M_b+L_b);
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;

  suffix = "test3";
  item = 20; // item needs to be >= history
  oct_str = "./test_gen_GMPvector.m " + to_string(K_a) + " " + to_string(L_a) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b) + " " + to_string(item) + " " + to_string(len) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re = (float *)malloc((len+history)*sizeof(float));
  in_re_fn = "in_re_" + suffix + ".txt"; 
  read_from_file(in_re_fn, len+history, in_re);
  
  // read imag part of input
  in_im = (float *)malloc((len+history)*sizeof(float));
  in_im_fn = "in_im_" + suffix + ".txt"; 
  read_from_file(in_im_fn, len+history, in_im);

  // read real part of output
  out_re = (float *)malloc((M+M_bar)*sizeof(float));
  out_re_fn = "out_re_" + suffix + ".txt"; 
  read_from_file(out_re_fn, M+M_bar, out_re);
  
  // read imag part of output
  out_im = (float *)malloc((M+M_bar)*sizeof(float));
  out_im_fn = "out_im_" + suffix + ".txt"; 
  read_from_file(out_im_fn, M+M_bar, out_im);

  // call gen_GMPvector() with the same input as octave function call 
  gr_complex in3[len+history];
  for (int ii = 0; ii < len+history; ii++)
    in3[ii] = gr_complex(in_re[ii], in_im[ii]);   
  vector<gr_complex> out3;
  out3.reserve(M+M_bar);         
  gen_GMPvector(in3, item-1, K_a, L_a+1, K_b, M_b, L_b+1, out3);

  /*
  for (int ii = 0; ii < M+M_bar; ii++)
    std::cout << "out_re[ii]: " << out3[ii].real() << "      out_im[ii]: " << out3[ii].imag() << std::endl;   
  */

  // check if gen_GMPvector.m output and gen_GMPvector.cc output are equal
  int test3_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equal( out3[kk].real(), out_re[kk], 1000.0 ) ) || ( !almost_equal( out3[kk].imag(), out_im[kk], 1000.0 ) )     ) {
      test3_pass = 0;       
      break;
    }
  }

  if (test3_pass)
    std::cout << "gen_GMPvector(): Test 3 Passed." << std::endl;
  else
    std::cerr << "gen_GMPvector(): Test 3 Failed." << std::endl;

  /* Test 4: Pre-Distorter */
  K_a = 5;
  L_a = 3;
  K_b = 7;
  L_b = 1; 
  M_b = 1;
  history = std::max(L_a, M_b+L_b);
  M = K_a*L_a+K_b*M_b*L_b;
  M_bar = K_a+K_b*M_b;

  suffix = "test4";
  item = 20; // item needs to be >= history
  oct_str = "./test_gen_GMPvector.m " + to_string(K_a) + " " + to_string(L_a-1) + " " + to_string(K_b) + " " + to_string(M_b) + " " + to_string(L_b-1) + " " + to_string(item) + " " + to_string(len) + " " + suffix;
  system( oct_str.c_str() );

  // read real part of input
  in_re = (float *)realloc(in_re, (len+history)*sizeof(float));
  in_re_fn = "in_re_" + suffix + ".txt"; 
  read_from_file(in_re_fn, len+history, in_re);
  
  // read imag part of input
  in_im = (float *)realloc(in_im, (len+history)*sizeof(float));
  in_im_fn = "in_im_" + suffix + ".txt"; 
  read_from_file(in_im_fn, len+history, in_im);

  // read real part of output
  out_re = (float *)realloc(out_re, M*sizeof(float));
  out_re_fn = "out_re_" + suffix + ".txt"; 
  read_from_file(out_re_fn, M, out_re);
  
  // read imag part of output
  out_im = (float *)realloc(out_im, M*sizeof(float));
  out_im_fn = "out_im_" + suffix + ".txt"; 
  read_from_file(out_im_fn, M, out_im);

  // call gen_GMPvector() with the same input as octave function call 
  gr_complex in4[len+history];
  for (int ii = 0; ii < len+history; ii++)
    in4[ii] = gr_complex(in_re[ii], in_im[ii]);   
  vector<gr_complex> out4;
  out4.reserve(M);         
  gen_GMPvector(in4, item-1, K_a, L_a, K_b, M_b, L_b, out4);

  /*
  for (int ii = 0; ii < M; ii++)
    std::cout << "out_re[ii]: " << out4[ii].real() << "      out_im[ii]: " << out4[ii].imag() << std::endl;   
  */

  // check if gen_GMPvector.m output and gen_GMPvector.cc output are equal
  int test4_pass = 1;
  for (int kk = 0; kk < M; kk++) {
    if (     ( !almost_equal( out4[kk].real(), out_re[kk], 1000.0 ) ) || ( !almost_equal( out4[kk].imag(), out_im[kk], 1000.0 ) )     ) {
      test4_pass = 0;       
      break;
    }
  }

  if (test4_pass)
    std::cout << "gen_GMPvector(): Test 4 Passed." << std::endl;
  else
    std::cerr << "gen_GMPvector(): Test 4 Failed." << std::endl;

  free(in_re);
  free(in_im);
  free(out_re);
  free(out_im);

  return 0;
}
