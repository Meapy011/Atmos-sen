#include <stdio.h>
#include <unistd.h>

#include "sensirion_i2c_hal.h"
#include "sensors.h"

#include "scd30_i2c.h"
#include "sen44_i2c.h"
#include "sfa3x_i2c.h"

#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec

static int init_scd30(void) {
    uint8_t major, minor;

    scd30_init(SCD30_I2C_ADDR_61);
    scd30_stop_periodic_measurement();
    scd30_soft_reset();
    sensirion_hal_sleep_us(2000000);

    if (scd30_read_firmware_version(&major, &minor) != NO_ERROR) {
        printf("SCD30: failed firmware read\n");
    } else {
        printf("SCD30 firmware: %u.%u\n", major, minor);
    }

    if (scd30_start_periodic_measurement(0) != NO_ERROR) {
        printf("SCD30: failed to start measurement\n");
        return -1;
    }

    return 0;
}

static int init_sen44(void) {
    if (sen44_start_measurement() != 0) {
        printf("SEN44: failed to start measurement\n");
        return -1;
    }
    return 0;
}

static int init_sfa3x(void) {
    int8_t marking[32] = {0};

    sfa3x_init(SFA3X_I2C_ADDR_5D);
    sfa3x_device_reset();
    sensirion_hal_sleep_us(100000);

    if (sfa3x_get_device_marking(marking, sizeof(marking)) == NO_ERROR)
        printf("SFA3X marking: %s\n", marking);

    if (sfa3x_start_continuous_measurement() != NO_ERROR) {
        printf("SFA3X: failed to start\n");
        return -1;
    }

    return 0;
}

int sensors_init_all(void) {

    printf("Initializing I2C HAL...\n");
    sensirion_i2c_hal_init();   // 🟢 FIXED: no return value

    printf("Initializing SCD30...\n");
    init_scd30();

    printf("Initializing SEN44...\n");
    init_sen44();

    printf("Initializing SFA3X...\n");
    init_sfa3x();

    return 0;
}

int sensors_read_all(sensor_data_t* out) {

    /* SCD30 */
    scd30_blocking_read_measurement_data(
        &out->co2, &out->temp_scd, &out->hum_scd);

    /* SEN44 */
    sen44_read_measured_values(
        &out->pm1, &out->pm25, &out->pm4, &out->pm10,
        &out->voc_index, &out->temp_sen, &out->hum_sen
    );

    /* SFA3X */
    float rh_unused, t_unused;
    sfa3x_read_measured_values(
        &out->hcho_ppb, &rh_unused, &t_unused
    );

    return 0;
}
