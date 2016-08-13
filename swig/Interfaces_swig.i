/* -*- c++ -*- */

#define INTERFACES_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "Interfaces_swig_doc.i"

%{
#include "Interfaces/length_framing_source_b.h"
#include "Interfaces/length_framing_sink_b.h"
#include "Interfaces/ax25_sink_b.h"
%}


%include "Interfaces/length_framing_source_b.h"
GR_SWIG_BLOCK_MAGIC2(Interfaces, length_framing_source_b);
%include "Interfaces/length_framing_sink_b.h"
GR_SWIG_BLOCK_MAGIC2(Interfaces, length_framing_sink_b);


%include "Interfaces/ax25_sink_b.h"
GR_SWIG_BLOCK_MAGIC2(Interfaces, ax25_sink_b);

