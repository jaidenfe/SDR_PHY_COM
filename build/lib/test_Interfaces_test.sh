#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/ground/jaidenfe/gr-Interfaces/lib
export PATH=/home/ground/jaidenfe/gr-Interfaces/build/lib:$PATH
export LD_LIBRARY_PATH=/home/ground/jaidenfe/gr-Interfaces/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-Interfaces 
