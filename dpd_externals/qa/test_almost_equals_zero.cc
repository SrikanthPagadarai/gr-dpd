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
#include <dpd_externals/almost_equals_zero.h>

int main(void) 
{
  /* Test 1 */
  double a = 999.9997, b = 1000.0005;
  if (almost_equals_zero(a-b, 3))
    std::cout << "almost_equals_zero(): Test 1 Passed." << std::endl; 
  else
    std::cerr << "almost_equals_zero(): Test 1 Failed." << std::endl;

  /* Test 2 */
  double c = 0.6348, d = 0.634884;
  if (almost_equals_zero(c-d, 4))
    std::cout << "almost_equals_zero(): Test 2 Passed." << std::endl; 
  else
    std::cerr << "almost_equals_zero(): Test 2 Failed." << std::endl;

  /* Test 3 */
  double e = -0.00058;  
  if (almost_equals_zero(e, 3))
    std::cout << "almost_equals_zero(): Test 2 Passed." << std::endl; 
  else
    std::cerr << "almost_equals_zero(): Test 2 Failed." << std::endl;

  return 0;
}
