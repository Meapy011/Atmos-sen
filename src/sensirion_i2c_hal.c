#include "sensirion_i2c_hal.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>

static int i2c_fd = -1;

void sensirion_i2c_hal_init(void) {
    const char *i2c_bus = "/dev/i2c-1"; // adjust for your platform
    i2c_fd = open(i2c_bus, O_RDWR);
    if (i2c_fd < 0) {
        perror("I2C open failed");
    }
}

void sensirion_i2c_hal_sleep_usec(uint32_t useconds) {
    usleep(useconds);
}

uint64_t sensirion_i2c_hal_get_time_usec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

int8_t sensirion_i2c_hal_write(uint8_t address, const uint8_t* data, uint16_t count) {
    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) return -1;
    if (write(i2c_fd, data, count) != count) return -1;
    return 0;
}

int8_t sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count) {
    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) return -1;
    if (read(i2c_fd, data, count) != count) return -1;
    return 0;
}
