#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include "sensirion_i2c_hal.h"
#include "sfa3x_i2c.h"
#include "scd30_i2c.h"
#include "sen44_i2c.h"
#include "sen66_i2c.h"

static volatile sig_atomic_t running = 1;

/* Timestamp "YYYY-MM-DD HH:MM:SS" */
static void get_local_timestamp(char* buf, size_t len) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm tm_info;
    if (localtime_r(&tv.tv_sec, &tm_info) == NULL) {
        snprintf(buf, len, "1970-01-01 00:00:00");
        return;
    }

    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm_info);
}

static void handle_signal(int sig) {
    (void)sig;
    running = 0;
}

int main(void) {

    setvbuf(stdout, NULL, _IOLBF, 0);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // INIT I2C
    sensirion_i2c_hal_init();

    // SFA3X
    sfa3x_init(SFA3X_I2C_ADDR_5D);
    sfa3x_device_reset();
    sensirion_i2c_hal_sleep_usec(1000000);
    sfa3x_start_continuous_measurement();

    // SCD30
    scd30_init(SCD30_I2C_ADDR_61);
    scd30_stop_periodic_measurement();
    scd30_soft_reset();
    sensirion_i2c_hal_sleep_usec(2000000);
    scd30_start_periodic_measurement(0);

    // SEN44
    sen44_device_reset();
    sen44_start_measurement();

    // SEN66
    sen66_init(SEN66_I2C_ADDR_6B);
    sen66_device_reset();
    sensirion_i2c_hal_sleep_usec(1200000);
    sen66_start_continuous_measurement();

    printf("Starting multi-sensor measurement loop...\n");

    while (running) {
        sensirion_i2c_hal_sleep_usec(1000000);

        char timestamp[32];
        get_local_timestamp(timestamp, sizeof(timestamp));
        printf("\n--- %s ---\n", timestamp);

        // SFA3X
        float hcho = 0.0f, sfa_hum = 0.0f, sfa_temp = 0.0f;
        if (!sfa3x_read_measured_values(&hcho, &sfa_hum, &sfa_temp)) {
            printf("SFA3X -> HCHO: %.2f ppb, Humidity: %.2f %%, Temp: %.2f °C\n",
                   hcho, sfa_hum, sfa_temp);
        }

        // SCD30
        float co2 = 0.0f, scd_temp = 0.0f, scd_hum = 0.0f;
        if (!scd30_blocking_read_measurement_data(&co2, &scd_temp, &scd_hum)) {
            printf("SCD30 -> CO2: %.2f ppm, Temp: %.2f °C, Humidity: %.2f %%\n",
                   co2, scd_temp, scd_hum);
        }

        // SEN44
        uint16_t pm1p0_44 = 0, pm2p5_44 = 0, pm4p0_44 = 0, pm10p0_44 = 0;
        float voc_44 = 0.0f, hum_44 = 0.0f, temp_44 = 0.0f;

        if (!sen44_read_measured_mass_concentration_and_ambient_values(
                &pm1p0_44, &pm2p5_44, &pm4p0_44, &pm10p0_44,
                &voc_44, &hum_44, &temp_44)) {

            printf("SEN44 -> PM1.0: %u, PM2.5: %u, PM4.0: %u, PM10: %u µg/m³\n",
                   pm1p0_44, pm2p5_44, pm4p0_44, pm10p0_44);
            printf("         VOC: %.1f, Humidity: %.2f %%, Temp: %.2f °C\n",
                   voc_44, hum_44, temp_44);
        }

        // SEN66
        float pm1p0_66 = 0.0f, pm2p5_66 = 0.0f, pm4p0_66 = 0.0f, pm10p0_66 = 0.0f;
        float hum_66 = 0.0f, temp_66 = 0.0f, voc_66 = 0.0f, nox_66 = 0.0f;
        uint16_t co2_66 = 0;

        if (!sen66_read_measured_values(
                &pm1p0_66, &pm2p5_66, &pm4p0_66, &pm10p0_66,
                &hum_66, &temp_66, &voc_66, &nox_66, &co2_66)) {

            printf("SEN66 -> PM1.0: %.1f, PM2.5: %.1f, PM4.0: %.1f, PM10: %.1f µg/m³\n",
                   pm1p0_66, pm2p5_66, pm4p0_66, pm10p0_66);
            printf("         Humidity: %.2f %%, Temp: %.2f °C, VOC: %.1f, NOx: %.1f, CO2: %u ppm\n",
                   hum_66, temp_66, voc_66, nox_66, co2_66);
        }
    }

    printf("Stopping measurements...\n");

    sfa3x_stop_measurement();
    scd30_stop_periodic_measurement();
    sen44_stop_measurement();
    sen66_stop_measurement();

    return 0;
}

