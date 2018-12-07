#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define DHT_NAME "/dev/dht_dev"

static int dht_result[4];
int* get_dht(void);
