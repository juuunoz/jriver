Juno's self-directed driver project with the endgoal of eventually supporting multiple custom gaming controllers on one computer to improve the local multiplayer videogame experience on PC.

This is the driver part of the project. I am also currently looking into ways to manipulate an arduino kit into a prototype game controller

resources used as of 2024/01/14: </br>
for general kernel module programming wisdom https://tldp.org/LDP/lkmpg/2.4/html/lkmpg.html#AEN34</br>
for less general driver module programming wisdom https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch03.html</br>
for basic c/linux and bash knowledge, CSCB09 at UTSC with Albert Lai</br>
for basic computer architecture knowledge, CSCB58 at UTSC with Nandita Vijaykumar</br>

Todo:
1. further reading on pros and cons of device port types (usb-c, usb, bluetooth docks) required
2. implement support for multiple device inputs, likely with semaphores and other fancy stuff

To use;

1. run load-devices to load devices
2. compile driver with make all
3. insert into kernel with insmod jriver.ko
4. run ```test <args>``` where arguments are the lines to be written and read into the device

To de-use;

1. remove module from kernel with rmmod jriver.ko
2. run unload-devices to unload devices (or just run rm /dev/jriver-device)

