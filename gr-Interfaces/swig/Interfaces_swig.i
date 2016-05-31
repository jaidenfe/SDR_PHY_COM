/* -*- c++ -*- */

#define INTERFACES_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "Interfaces_swig_doc.i"

%{
#include "Interfaces/queue_len_deframer_sink_b.h"
#include "Interfaces/queue_len_framer_source_b.h"
%}


%include "Interfaces/queue_len_deframer_sink_b.h"
GR_SWIG_BLOCK_MAGIC2(Interfaces, queue_len_deframer_sink_b);
%include "Interfaces/queue_len_framer_source_b.h"
GR_SWIG_BLOCK_MAGIC2(Interfaces, queue_len_framer_source_b);
