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

#ifndef INCLUDED_DPD_PREDISTORTER_TRAINING_IMPL_H
#define INCLUDED_DPD_PREDISTORTER_TRAINING_IMPL_H

#include <dpd/predistorter_training.h>
#include <armadillo>

namespace gr {
  namespace dpd {

    class predistorter_training_impl : public predistorter_training
    {
     private:
      int d_NFFT, d_cp_len, d_ovx, d_num_zero_syms, d_M, nskip;
      int relative_sample_index, sent_sample_index, current_ofdm_block_index;        
      bool d_update_predistorter_training, update_predistorter_training; 
      arma::cx_colvec d_predistorter_training_colvec, predistorter_training_colvec;     
      std::ofstream log_file; 
      int d_save_log;
      uint64_t nread;
      std::vector<gr::tag_t> tags; // container to hold tags      

     public:
      predistorter_training_impl(int NFFT, int cp_len, int ovx, int num_zero_syms, int M, int save_log);
      ~predistorter_training_impl();

      void get_taps(pmt::pmt_t P);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_PREDISTORTER_TRAINING_IMPL_H */

