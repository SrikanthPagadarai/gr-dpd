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
