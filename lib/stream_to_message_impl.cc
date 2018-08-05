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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "stream_to_message_impl.h"

namespace gr {
  namespace dpd {

    stream_to_message::sptr
    stream_to_message::make(int msg_len)
    {
      return gnuradio::get_initial_sptr
        (new stream_to_message_impl(msg_len));
    }

    /*
     * The private constructor
     */
    stream_to_message_impl::stream_to_message_impl(int msg_len)
      : gr::sync_block("stream_to_message",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0)),
              d_msg_len(msg_len)
    {
      samples.reserve(d_msg_len);
      message_port_register_out(pmt::mp("samples"));
    }

    /*
     * Our virtual destructor.
     */
    stream_to_message_impl::~stream_to_message_impl()
    {
    }

    int
    stream_to_message_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];

      // Do <+signal processing+>
      static int count = 0;
      static bool msg_not_sent = true;

      for (int i = 0; i < noutput_items && msg_not_sent; i++) 
      {
        samples.push_back(in[i]);
	count++;	  

        if (count == d_msg_len) 
        {
          pmt::pmt_t P_c32vector_samples = pmt::init_c32vector(d_msg_len, samples);
          message_port_pub(pmt::mp("samples"), P_c32vector_samples);

          msg_not_sent = false;
	}
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dpd */
} /* namespace gr */

