#!/bin/bash
mkdir build
cd build
sudo make uninstall
cmake ..
make
sudo make install
