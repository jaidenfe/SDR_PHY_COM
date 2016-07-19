<h1>gr-Interfaces</h1>
>University at Buffalo Nanosatellite Laboratory 2016<br>
>Created By Jaiden Ferraccioli<br>
>E-mail: jaidenfe@buffalo.edu<br>
>Current Version:    v0.0.1<br>
<!--
<br>
[![Build Status](https://travis-ci.org/jaidenfe/SDR_PHY_COM.svg?branch=master)](https://travis-ci.org/jaidenfe/SDR_PHY_COM)
-->

gr-Interfaces is a physical layer interface module between GNU Radio and other software applications. It was designed to be used with a Software Defined Radio (SDR) and custom python applications. These blocks offer full or half duplex packet radio capabilities to developer projects. 

<h3>Dependencies</h3>
The following programs are required for gr-Interfaces:
<ul>
  <li> <a href="http://gnuradio.org/redmine/projects/gnuradio/wiki/Download">GNU Radio 3.7.10</a><br>
  <li> Python 2.7 <br>
</ul>

This module is used to extend the functionality of GNU Radio. These blocks were designed for use with other applications that require packet radio functionality and were developed using GNU Radio 3.7.10 and Python 2.7.

<h3>Content</h3>
The gr-Interfaces module contains several interface blocks, with more to come, that help integrate developers software with GNU Radio. Here is a list of the blocks contained within gr-Interfaces with a description of their functionality:
<ul>
  <li><b>AX.25 Source -</b> Allows for AX.25 protocol to be implemented with an easy to use transmission queue interface</li>
  <li><b>AX.25 Sink -</b> Receives AX.25 packets, checks their CRC values and stores the payload in the receive queue</li>
  <li><b>Length Framing Source -</b> Frames packets contained within the transmission queue using length-based framing</li>
  <li><b>Length Framing Sink -</b> Receives framed packets using length-based framing techniques and stores the packet in the receive queue</li>
</ul>

<h3>Installation</h3>

There are two ways to install the gr-Interfaces module. The easiest way to install is to run the _install.sh_ script from your terminal. 

```$ ./install.sh```

The other, more tedious way to install this module is to follow the instructions below:

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

<h3>Uninstalling</h3>

To uninstall the blocks change into the ```gr-Interfaces/build``` directory
<br>```$ cd PATH/TO/DIR/gr-Interfaces/build```

then uninstall using
<br>```$ sudo make uninstall```

from here you can remake the gr-Interfaces library by following the installation section above or just delete the entire directory by changing into the directory that contains gr-Interfaces and recursively deleting all files
<br>```$ sudo rm -rf gr-Interfaces```

