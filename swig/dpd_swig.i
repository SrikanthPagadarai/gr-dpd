/* -*- c++ -*- */

#define DPD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dpd_swig_doc.i"

%{
#include "dpd/peak_detect.h"
#include "dpd/sts_blocker.h"
%}


%include "dpd/peak_detect.h"
GR_SWIG_BLOCK_MAGIC2(dpd, peak_detect);
%include "dpd/sts_blocker.h"
GR_SWIG_BLOCK_MAGIC2(dpd, sts_blocker);
