#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SOIL1_DEV_NAME "/dev/soil_dev"
#define SOIL2_DEV_NAME "/dev/soil_dev_2"

int status_soil(int mode);
