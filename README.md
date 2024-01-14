Juno's self-directed driver project with the goal of eventually supporting multiple gaming controllers on one computer with the goal of improving the local multiplayer videogame experience.

To use; 
1. run load-devices to load devices
2. compile driver with make all
3. insert into kernel with insmod jriver.ko

To de-use;
1. remove module from kernel with rmmod jriver.ko
2. run unload-devices to unload devices (it actually deletes them)
