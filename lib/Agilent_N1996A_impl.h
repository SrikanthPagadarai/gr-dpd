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

#ifndef INCLUDED_DPD_AGILENT_N1996A_IMPL_H
#define INCLUDED_DPD_AGILENT_N1996A_IMPL_H

#include <dpd/Agilent_N1996A.h>
#include <string>
#include <gnuradio/thread/thread.h>
#include "vxi11_user.h"

namespace gr {
  namespace dpd {

    class Agilent_N1996A_impl : public Agilent_N1996A
    {
     private:
      unsigned long int d_frequency;    // Center frequency in Hz
      unsigned long int d_span;         // Span in Hz
      unsigned int d_resbw;             // Resolution bandwidth in Hz
      unsigned int d_nb_points;         // Number of x-axis points on PSD plot
      std::string d_ip_addr;            // N1996A IP Address
      CLINK d_vxi_link;                 // VXI-11 instrument handler
      gr::thread::mutex d_1996a_mutex;  // N1996A communication protection
      char *d_n1996a_buf;               // Buffer for PSD data block returned by N1996A
      size_t d_n1996a_bufsize;          // Size of buffer used for wave data retrieval from N1996A
      FILE *fp_SA_meas;
      char filename[255];
      time_t now;   
      int count, i, j;
      char counter_str[8]; 
      unsigned int counter_size;
      unsigned int byte_count;                    
      
     public:
      Agilent_N1996A_impl(const std::string& ip_addr, float frequency, float span, float resbw, uint32_t nb_points);
      ~Agilent_N1996A_impl();

      void set_frequency(float frequency);
      void set_span(float span);
      void set_resbw(float resbw);
      void set_sweep_points(unsigned int nb_points);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
      

 
      // send command to instrument utility
      void send_command(const char *command, bool protect = true);

      // send command with unsigned integer argument to instrument utility
      void send_command_u(const char *command, unsigned int value);

      // send command with unsigned long integer argument to instrument utility
      void send_command_ul(const char *command, unsigned long int value);

      // send command and get response from instrument 
      size_t send_command_and_get_response(const char *command, char *buf, 
            const size_t bufsize, bool protect = true);

      // send command and get response from instrument as a double value
      void send_command_and_get_response_double(const char *command, 
            double *value);

      // send command and get response from instrument as an unsigned int value
      void send_command_and_get_response_int(const char *command, 
            int *value);

      // send command and print response
      void send_command_and_print_message(const char *command);

    };

  } // namespace dpd
} // namespace gr

#endif /* INCLUDED_DPD_AGILENT_N1996A_IMPL_H */

