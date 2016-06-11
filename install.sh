#!/bin/sh
cd gr-Interfaces
#mkdir build
cd build
make uninstall
cmake ..
make
make install
echo "Install Complete"
