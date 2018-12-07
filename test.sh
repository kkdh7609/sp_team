#!/bin/bash
sudo insmod ./button/button_dev.ko
sudo insmod ./button/button_dev_2.ko
sudo mknod -m 666 /dev/button_dev c 239 0
sudo mknod -m 666 /dev/button_dev_2 c 238 0
sudo insmod ./light/light_dev.ko
sudo mknod -m 666 /dev/light_dev c 243 0
