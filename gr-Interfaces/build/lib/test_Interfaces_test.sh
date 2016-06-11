#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/ubnl-sof/COM/SDR_PHY_COM/gr-Interfaces/lib
export GR_CONF_CONTROLPORT_ON=False
export PATH=/home/ubnl-sof/COM/SDR_PHY_COM/gr-Interfaces/build/lib:$PATH
export LD_LIBRARY_PATH=/home/ubnl-sof/COM/SDR_PHY_COM/gr-Interfaces/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-Interfaces 
