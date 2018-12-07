cd button
sudo insmod button_dev.ko
sudo mknod -m 666 /dev/button_dev c 239 0
cd ..
cd light
sudo insmod light_dev.ko
sudo mknod -m 666 /dev/light_dev c 243 0
cd ..
cd main