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
              gr::io_signature::make(1, 1, nb_points*sizeof(float))),
              d_ip_addr(ip_addr),
              d_frequency(frequency),
              d_span(span),
              d_resbw(resbw),
              d_nb_points(2*nb_points)              
    {
      d_n1996a_bufsize = d_nb_points*8;
      d_n1996a_buf = (char *) malloc(d_n1996a_bufsize);

      int ret = vxi11_open_device(ip_addr.c_str(), &d_vxi_link); // try to establish VXI-11 communication link with N1996A
    
      if (ret != 0) 
      {
        std::cerr << "N1996A: cannot open VXI-11 channel" << std::endl;
        throw std::runtime_error("cannot open VXI-11 channel");
      }
      else
      {
        std::cout << "N1996A: VXI-11 channel opened" << std::endl;
        std::cout << "Instrument Identification Information: "; 
        send_command_and_print_message("*IDN?");
      }

      // setup 
      send_command(":INST:SEL SA");      // set analayzer to SA mode
      send_command(":INIT:CONT 0");
      set_frequency(d_frequency);        // set center frequency
      set_span(d_span);                  // set span
      set_sweep_points(d_nb_points);
      send_command(":FORM:BORD SWAP");   // set binary byte order to SWAP
      send_command(":FORM REAL,32");     // set ouput format to a binary 32 bit format
    }

    /*
     * Our virtual destructor.
     */
    Agilent_N1996A_impl::~Agilent_N1996A_impl()
    {
      // send_command(":INIT:CONT 1"); // restore continuous measurement
      // send_command(":CAL:AUTO ON"); // restore auto calibration
    
      vxi11_close_device(d_ip_addr.c_str(), &d_vxi_link); // close VXI-11 communication link with N1996A
    }


    // send command to instrument utility
    void Agilent_N1996A_impl::send_command(const char *command, bool protect)
    {
      int ret;
 
      if (protect)
      {
        gr::thread::scoped_lock guard(d_1996a_mutex); // protect communication with N1996A
        ret = vxi11_send(&d_vxi_link, command);
      }

      if (ret < 0)
      {
        std::cerr << "N1996A: cannot send command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot send command to N1996A");
      }    
    }

    // send command with unsigned integer argument to instrument utility
    void Agilent_N1996A_impl::send_command_u(const char *command, unsigned int value)
    {
      char cmd[64];
    
      sprintf(cmd, "%s %u", command, value);
      send_command(cmd);
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
        char *buf, const size_t bufsize, bool protect)
    {
      int ret, bytes_returned;
      if (protect)
      {
        gr::thread::scoped_lock guard(d_1996a_mutex); // protect communication with N1996A
        ret = vxi11_send(&d_vxi_link, command);
        bytes_returned = vxi11_receive(&d_vxi_link, buf, bufsize);
      }

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

    // send command and get response from instrument as an unsigned int value
    void Agilent_N1996A_impl::send_command_and_get_response_int(const char *command, 
        int *value)
    {
      const size_t rcvsize = 64;
      char rcv[rcvsize];
    
      size_t bytes_returned = send_command_and_get_response(command, rcv, rcvsize);
      rcv[bytes_returned] = '\0';
      sscanf(rcv, "%d", value);
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

    // send command and get response from instrument and display message
    void Agilent_N1996A_impl::send_command_and_print_message(const char *command)
    {
      const size_t rcvsize = 64;
      char rcv[rcvsize];
    
      size_t bytes_returned = send_command_and_get_response(command, rcv, rcvsize);
      printf("%s\n",rcv);
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
          send_command(":INIT:CONT 0");

          // fetch data from N1996A and do signal processing    
          size_t bytes_returned = send_command_and_get_response("TRAC:DATA? TRACE1", d_n1996a_buf, d_n1996a_bufsize);
    
          if (bytes_returned < 2)
          {
             std::cerr << "N1996A: PSD data size less than 2 bytes (" << bytes_returned << " bytes)" << std::endl;
             throw std::runtime_error("invalid size for PSD data");
          }
          else
          {
             char counter_str[8];
        
             if (d_n1996a_buf[0] != '#')
             {
               std::cerr << "N1996A: invalid magic character for PSD data (" << d_n1996a_buf[0] << " bytes)" << std::endl;
               throw std::runtime_error("invalid magic character for PSD data");
             }
        
             strncpy(counter_str, &d_n1996a_buf[1], 1);
             counter_str[1] = '\0';
        
             unsigned int counter_size = strtoul(counter_str, 0, 10);
        
             if (bytes_returned < 2+counter_size)
             {
               std::cerr << "N1996A: PSD data size less than header data size (" << bytes_returned << " bytes)" << std::endl;
               throw std::runtime_error("invalid size for PSD data");
             }
             else
             {
               strncpy(counter_str, &d_n1996a_buf[2], counter_size);
               counter_str[counter_size] = '\0';
            
               unsigned int  byte_count = strtoul(counter_str, 0, 10);
            
               if (bytes_returned < 2+counter_size+byte_count)
               {
                 std::cerr << "N1996A: PSD data block truncated to " << bytes_returned-2-counter_size << " bytes" << std::endl;
                 throw std::runtime_error("invalid size for PSD data");
               }
               else
               {
                 float *iq_array = (float *) &d_n1996a_buf[2+counter_size];
                
                 for (int j = 1; j < d_nb_points; j+=2)                 
                    out[i*d_nb_points+j] = iq_array[j];
                
                 // noutput_items = d_nb_points;
               }


             }
           
          }

          /*
          for (int j = 0; j < d_nb_points; j++)
             out[i*d_nb_points+j] = 0.0;
          */
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

