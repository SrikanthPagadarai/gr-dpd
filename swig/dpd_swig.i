/* -*- c++ -*- */

#define DPD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dpd_swig_doc.i"

%{
#include "dpd/peak_detect.h"
#include "dpd/sts_blocker.h"
#include "dpd/predistorter.h"
#include "dpd/stream_to_zero_padded_vector.h"
#include "dpd/add_cp.h"
#include "dpd/Agilent_N1996A.h"
#include "dpd/polynomial_regressor_vector.h"
#include "dpd/signum_func.h"
#include "dpd/gain_phase_calibrate.h"
#include "dpd/postdistorter.h"
#include "dpd/stream_to_message.h"
%}


%include "dpd/peak_detect.h"
GR_SWIG_BLOCK_MAGIC2(dpd, peak_detect);
%include "dpd/sts_blocker.h"
GR_SWIG_BLOCK_MAGIC2(dpd, sts_blocker);
%include "dpd/predistorter.h"
GR_SWIG_BLOCK_MAGIC2(dpd, predistorter);
%include "dpd/stream_to_zero_padded_vector.h"
GR_SWIG_BLOCK_MAGIC2(dpd, stream_to_zero_padded_vector);
%include "dpd/add_cp.h"
GR_SWIG_BLOCK_MAGIC2(dpd, add_cp);
%include "dpd/Agilent_N1996A.h"
GR_SWIG_BLOCK_MAGIC2(dpd, Agilent_N1996A);
%include "dpd/polynomial_regressor_vector.h"
GR_SWIG_BLOCK_MAGIC2(dpd, polynomial_regressor_vector);
%include "dpd/signum_func.h"
GR_SWIG_BLOCK_MAGIC2(dpd, signum_func);
%include "dpd/gain_phase_calibrate.h"
GR_SWIG_BLOCK_MAGIC2(dpd, gain_phase_calibrate);

%include "dpd/postdistorter.h"
GR_SWIG_BLOCK_MAGIC2(dpd, postdistorter);
%include "dpd/stream_to_message.h"
GR_SWIG_BLOCK_MAGIC2(dpd, stream_to_message);
