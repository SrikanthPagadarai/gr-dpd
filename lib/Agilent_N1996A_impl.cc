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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Agilent_N1996A_impl.h"

namespace gr {
  namespace dpd {

    Agilent_N1996A::sptr
    Agilent_N1996A::make(const std::string& ip_addr, float frequency, float span, float resbw, uint32_t nb_points)
    {
      return gnuradio::get_initial_sptr
        (new Agilent_N1996A_impl(ip_addr, frequency, span, resbw, nb_points));
    }

    /*
     * The private constructor
     */
    Agilent_N1996A_impl::Agilent_N1996A_impl(const std::string& ip_addr, float frequency, float span, float resbw, uint32_t nb_points)
      : gr::sync_block("Agilent_N1996A",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_ip_addr(ip_addr),
              d_frequency(frequency),
              d_span(span),
              d_resbw(resbw),
              d_nb_points(nb_points)              
    {
      int ret = vxi11_open_device(ip_addr.c_str(), &d_vxi_link); // try to establish VXI-11 communication link with E4406A
    
      if (ret != 0) 
      {
        std::cerr << "N1996A: cannot open VXI-11 channel" << std::endl;
        throw std::runtime_error("cannot open VXI-11 channel");
      }
      else
        std::cout << "N1996A: VXI-11 channel opened" << std::endl;

      // send_command(":DISP:ENAB 0"); // inhibit display

      send_command(":SYST:MESS \"In Use by GNU Radio - front panel disabled\""); // send informatory message to N1996A status line

      set_frequency(d_frequency);
      set_span(d_span);
      set_sweep_points(d_nb_points);
    }

    /*
     * Our virtual destructor.
     */
    Agilent_N1996A_impl::~Agilent_N1996A_impl()
    {
      send_command(":DISP:ENAB 1"); // release display
      send_command(":SYST:KLOC 0"); // release keyboard
      send_command(":INIT:CONT 1"); // restore continuous measurement
      send_command(":CAL:AUTO ON"); // restore auto calibration
    
      // vxi11_close_device(d_ip_addr.c_str(), &d_vxi_link); // close VXI-11 communication link with N1996A
    }


    // send command to instrument utility
    void Agilent_N1996A_impl::send_command(const char *command)
    {
      int ret;
 
      ret = vxi11_send(&d_vxi_link, command);
      
      if (ret < 0)
      {
        std::cerr << "N1996A: cannot send command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot send command to N1996A");
      }    
    }


    // send command with unsigned long integer argument to instrument utility
    void Agilent_N1996A_impl::send_command_ul(const char *command, unsigned long int value)
    {
      char cmd[64];
    
      sprintf(cmd, "%s %lu", command, value);
      send_command(cmd);
    }


    // send command and get response from instrument 
    size_t Agilent_N1996A_impl::send_command_and_get_response(const char *command, 
        char *buf, const size_t bufsize)
    {
      int ret, bytes_returned;
    
      ret = vxi11_send(&d_vxi_link, command);
      bytes_returned = vxi11_receive(&d_vxi_link, buf, bufsize);
    
      if (ret < 0)
      {
        std::cerr << "N1996A: cannot send command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot send command to N1996A");
      }
      else if (bytes_returned > 0)
      {
        return bytes_returned;
      }
      else
      {
        std::cerr << "N1996A: no reply to command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot get reply from N1996A");
      }    
    }


    // send command and get response from instrument as a double value
    void Agilent_N1996A_impl::send_command_and_get_response_double(const char *command, 
        double *value)
    {
      const size_t rcvsize = 64;
      char rcv[rcvsize];
    
      size_t bytes_returned = send_command_and_get_response(command, rcv, rcvsize);
      rcv[bytes_returned] = '\0';
      sscanf(rcv, "%lf", value);
    }

    // set center frequency
    void Agilent_N1996A_impl::set_frequency(float frequency)
    {
      double n1996a_freq;
    
      d_frequency = frequency;
      send_command_ul(":FREQ:CENT", d_frequency); // set center frequency
      send_command_and_get_response_double(":FREQ:CENT?", &n1996a_freq); 
      d_frequency = n1996a_freq; // update with frequency as set by the N1996A
    }

    // set span
    void Agilent_N1996A_impl::set_span(float span)
    {
      double n1996a_span;
    
      d_span = span;
      send_command_ul(":FREQ:SPAN", d_span); // set span
      send_command_and_get_response_double(":FREQ:SPAN?", &n1996a_span); 
      d_span = n1996a_span; // update with span as set by the N1996A
    }


    // set number of sweep points
    void Agilent_N1996A_impl::set_sweep_points(unsigned int nb_points)
    {
      double n1996a_nb_points;
    
      d_nb_points = nb_points;
      send_command_ul(":SWE:POIN", d_nb_points); // set span
    }

    int
    Agilent_N1996A_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int i = 0; i < noutput_items; i++)
      {
          out[i].real() = 0.0;
          out[i].imag() = 0.0;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

