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

#ifndef INCLUDED_DPD_STS_BLOCKER_IMPL_H
#define INCLUDED_DPD_STS_BLOCKER_IMPL_H

#include <dpd/sts_blocker.h>

namespace gr {
  namespace dpd {

    class sts_blocker_impl : public sts_blocker
    {
     private:
      int d_expected_peaks, peaks, items_to_copy;
      std::vector<gr::tag_t> tags; // container to hold tags
      const char *iptr;
      char *optr;

     public:
      sts_blocker_impl();
      ~sts_blocker_impl();

      // Where all the action really happens
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_STS_BLOCKER_IMPL_H */

