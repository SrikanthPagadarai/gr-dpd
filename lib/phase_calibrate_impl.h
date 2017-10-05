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

#ifndef INCLUDED_DPD_PHASE_CALIBRATE_IMPL_H
#define INCLUDED_DPD_PHASE_CALIBRATE_IMPL_H

#include <dpd/phase_calibrate.h>

namespace gr {
  namespace dpd {

    class phase_calibrate_impl : public phase_calibrate
    {
     private:
      bool d_reference_acquired;
      std::vector<gr_complex> d_reference_samples;
      int d_ref_len, current_sample_index;
      bool packet_start;
      std::vector<gr::tag_t> tags; // container to hold tags

     public:
      phase_calibrate_impl(int ref_len);
      ~phase_calibrate_impl();

      void get_reference(pmt::pmt_t P); 

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_PHASE_CALIBRATE_IMPL_H */

