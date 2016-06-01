<h1>SDR Physical Layer Interfaces</h1>

SDR_PHY_COM is a physical layer interface between a Software Defined Radio (SDR) and other layers within an application or system architecture. This program uses gnuradio as the signal processing engine and is set up to operate in full duplex.

INSTALLATION:

In order to install the gr-Interfaces blocks (queue length-based framer source and sink) follow the instructions below:

Open a terminal window and change into the gr-Interfaces directory.

Make a directory called build
$ mkdir build

Then change into the directory and cmake the parent directory
$ cd build
$ cmake ..

Then make the blocks
$ make

If the compilation was successful you can install the blocks using the 'sudo make install' command
$ sudo make install

You can now open gnuradio and use the gr-Interfaces blocks under the INTERFACES tab.

UNINSTALLING GR-INTERFACES:

To uninstall the blocks change into the gr-Interfaces/build directory
$ cd PATH/TO/DIR/gr-Interfaces/build

then use the 'sudo make uninstall' command
$ sudo make uninstall

from here you can remake the gr-Interfaces library by following the installation section from the 'cmake' step above or just delete the entire directory by changing into the directory that contains gr-Interfaces and running the command
$ sudo rm -rf gr-Interfaces

