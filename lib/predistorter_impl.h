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

#ifndef INCLUDED_DPD_PREDISTORTER_IMPL_H
#define INCLUDED_DPD_PREDISTORTER_IMPL_H

#include <dpd/predistorter.h>
#include <armadillo>

namespace gr {
  namespace dpd {

    class predistorter_impl : public predistorter
    {
     private:
      int d_NFFT;
      int d_cp_len;
      int d_ovx;
      int d_num_zero_syms; 
      int d_M;
      bool d_update_predistorter; 
      arma::cx_colvec d_predistorter_colvec, predistorter_colvec;     
      std::ofstream log_file; 
      int d_save_log;
      bool update_predistorter;
      std::vector<gr::tag_t> tags; // container to hold tags
      int current_sample_index, sent_sample_index, current_ofdm_block_index;        

     public:
      predistorter_impl(int NFFT, int cp_len, int ovx, int num_zero_syms, int M, int save_log);
      ~predistorter_impl();

      void set_taps(pmt::pmt_t P);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_PREDISTORTER_IMPL_H */

