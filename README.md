<h1>SDR Physical Layer Interfaces</h1>

SDR_PHY_COM is a physical layer interface between a Software Defined Radio (SDR) and other layers within an application or system architecture. This program uses gnuradio as the signal processing engine and is set up to operate in full duplex.

<h3>Installation:</h3>

In order to install the gr-Interfaces blocks (queue length-based framer source and sink) follow the instructions below:

Open a terminal window and change into the gr-Interfaces directory.

Make a directory called build
<br>$ mkdir build

Then change into the directory and cmake the parent directory
<br>$ cd build
<br>$ cmake ..

Then make the blocks
<br>$ make

If the compilation was successful you can install the blocks using the 'sudo make install' command
<br>$ sudo make install

You can now open gnuradio and use the gr-Interfaces blocks under the INTERFACES tab.

<h3>Uninstalling:</h3>

To uninstall the blocks change into the gr-Interfaces/build directory
<br>$ cd PATH/TO/DIR/gr-Interfaces/build

then use the 'sudo make uninstall' command
<br>$ sudo make uninstall

from here you can remake the gr-Interfaces library by following the installation section from the 'cmake' step above or just delete the entire directory by changing into the directory that contains gr-Interfaces and running the command
<br>$ sudo rm -rf gr-Interfaces

