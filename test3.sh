#!/bin/bash
sudo insmod ./gas/gas_dev.ko
sudo mknod -m 666 /dev/gas_dev c 240 0
sudo insmod ./Soilmoisture/Soilmoisture_dev.ko
sudo insmod ./Soilmoisture/Soilmoisture_dev_2.ko
sudo mknod -m 666 /dev/soil_dev c 241 0
sudo mknod -m 666 /dev/soil_dev c 242 0
sudo insmod ./dht/dht_dev.ko
sudo mknod -m 666 /dev/dht_dev c 239 0
