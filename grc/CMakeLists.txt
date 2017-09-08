# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    dpd_peak_detect.xml
    dpd_sts_blocker.xml
    dpd_predistorter.xml
    dpd_stream_to_zero_padded_vector.xml
    dpd_add_cp.xml
    dpd_Agilent_N1996A.xml
    dpd_polynomial_regressor_vector.xml
    dpd_signum_func.xml
    dpd_gain_phase_calibrate.xml
    dpd_fastRLS_DPD.xml 
    dpd_ampl_diff.xml
    dpd_estimate_integer_delay.xml
    dpd_pa_wiener_model.xml
    dpd_basic_qpskofdm_tx_withoutCP.xml
    dpd_STS_generator_withoutCP.xml
    dpd_upsample.xml
    dpd_stream_to_message.xml DESTINATION share/gnuradio/grc/blocks
)
