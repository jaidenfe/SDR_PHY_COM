<h1>SDR Physical Layer Interface</h1>
>University at Buffalo Nanosatellite Laboratory 2016<br>
>Created By Jaiden Ferraccioli<br>
>E-mail: jaidenfe@buffalo.edu<br>
>Current Version:    v1.0<br>

SDR_PHY_COM is a physical layer interface between a Software Defined Radio (SDR) and other layers within an application or system architecture. This program uses GNU Radio as the signal processing engine and is set up to operate in full duplex.

<h3>Dependencies</h3>
The following programs are required to use the SDR Physical Layer Interfaces:
<ul>
  <li> GNU Radio 3.7.9.1 <br>
  <li> Python 2.7 <br>
</ul>

This library is used to extend the functionality of GNU Radio. These blocks were designed for use with other applications that require packet radio functionality and were developed using GNU Radio 3.7.9.1 and Python 2.7.

<h3>Installation:</h3>

In order to install the gr-Interfaces blocks (queue length-based framer source and sink) follow the instructions below:

Open a terminal window and change into the gr-Interfaces directory.

Make a directory called build
<br>```$ mkdir build```

Then change into the directory and cmake the parent directory
<br>```$ cd build```
<br>```$ cmake ..```

Then make and install the blocks
<br>```$ make```
<br>```$ sudo make install```

You can now open gnuradio-companion and use the gr-Interfaces blocks under the <b>INTERFACES</b> tab.

<h3>Uninstalling:</h3>

To uninstall the blocks change into the ```gr-Interfaces/build``` directory
<br>```$ cd PATH/TO/DIR/gr-Interfaces/build```

then uninstall using
<br>```$ sudo make uninstall```

from here you can remake the gr-Interfaces library by following the installation section above or just delete the entire directory by changing into the directory that contains gr-Interfaces and recursively deleting all files
<br>```$ sudo rm -rf gr-Interfaces```

