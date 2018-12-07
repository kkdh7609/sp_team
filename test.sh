cd button
sudo insmod button_dev.ko
sudo mknod -m 666 /dev/button_dev c 
cd ..
cd light
sudo insmod light_dev.ko
sudo mknod -m 666 /dev/light_dev c
cd ..
cd servo180
sudo insmod servo180_dev.ko
sudo mknod -m 666 /dev/servo180_dev c
cd ..
cd main