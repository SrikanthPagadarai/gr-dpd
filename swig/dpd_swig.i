/* -*- c++ -*- */

#define DPD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dpd_swig_doc.i"

%{
#include "dpd/peak_detect.h"
#include "dpd/sts_blocker.h"
#include "dpd/predistorter_training.h"
#include "dpd/stream_to_zero_padded_vector.h"
#include "dpd/Agilent_N1996A.h"
#include "dpd/stream_to_gmp_vector.h"
#include "dpd/signum_func.h"
#include "dpd/gain_phase_calibrate.h"
#include "dpd/postdistorter.h"
#include "dpd/stream_to_message.h"
#include "dpd/vector_vector_multiply.h"
%}


%include "dpd/peak_detect.h"
GR_SWIG_BLOCK_MAGIC2(dpd, peak_detect);
%include "dpd/sts_blocker.h"
GR_SWIG_BLOCK_MAGIC2(dpd, sts_blocker);
%include "dpd/predistorter_training.h"
GR_SWIG_BLOCK_MAGIC2(dpd, predistorter_training);
%include "dpd/stream_to_zero_padded_vector.h"
GR_SWIG_BLOCK_MAGIC2(dpd, stream_to_zero_padded_vector);

%include "dpd/Agilent_N1996A.h"
GR_SWIG_BLOCK_MAGIC2(dpd, Agilent_N1996A);
%include "dpd/stream_to_gmp_vector.h"
GR_SWIG_BLOCK_MAGIC2(dpd, stream_to_gmp_vector);
%include "dpd/signum_func.h"
GR_SWIG_BLOCK_MAGIC2(dpd, signum_func);
%include "dpd/gain_phase_calibrate.h"
GR_SWIG_BLOCK_MAGIC2(dpd, gain_phase_calibrate);

%include "dpd/postdistorter.h"
GR_SWIG_BLOCK_MAGIC2(dpd, postdistorter);
%include "dpd/stream_to_message.h"
GR_SWIG_BLOCK_MAGIC2(dpd, stream_to_message);
%include "dpd/vector_vector_multiply.h"
GR_SWIG_BLOCK_MAGIC2(dpd, vector_vector_multiply);

