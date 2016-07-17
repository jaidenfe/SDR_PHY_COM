#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/ground/jaidenfe/SDR_PHY_COM/gr-Interfaces/lib
export GR_CONF_CONTROLPORT_ON=False
export PATH=/home/ground/jaidenfe/SDR_PHY_COM/gr-Interfaces/build/lib:$PATH
export LD_LIBRARY_PATH=/home/ground/jaidenfe/SDR_PHY_COM/gr-Interfaces/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-Interfaces 
