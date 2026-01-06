# Ras-Sensirion: C code for Sensirion Sensor Read out on Pi Terminal

<center><img src="imgs/raspi.png" width=800px" ><center>

This document explains how to set up the Sen sensors to run on a Raspberry Pi using the provided code, and printing the readings to the terminal.

## Supported sensors

- SFA3X
- SCD30
- SEN44 
- SEN55
- SEN66

## Setup Guide

### Connecting the Sensor

The sensor board cable has the five different connectors: VCC, GND, SDA, SCL, GND
Use the following pins to connect your Board

 *Board*  |    *Raspberry Pi*           |   *Jumper Wire*   |
 :------: | :-------------------------: | :---------------: |
   VCC    |        Pin 2 (5V)           |   Red             |
   GND    |        Pin 6                |   Black           |
   SDA    |        Pin 3                |   Green           |
   SCL    |        Pin 5                |   Yellow          |
   SEL    |        Pin 9 (GND for I2C)  |   Blue            |

<center><img src="imgs/GPIO-Pinout-Diagram.png" width="900px"></center>

<center><img src="imgs/Board-Pinout-Diagram.png" width="900px"></center>

| *Pin* | *Name* | *Description*                   | *Comments*                       |
|-------|--------|---------------------------------|----------------------------------|
| 1     | GND    | Ground                          |                                  |
| 2     | GND    | Ground                          |                                  |
| 3     | SCL    | I2C: Serial clock input         | TTL 5V and LVTTL 3.3V compatible |
| 4     | SDA    | I2C: Serial data input / output | TTL 5V and LVTTL 3.3V compatible |
| 5     | NC     | Do not connect                  |                                  |


### Raspberry Pi

- [Install the Raspberry Pi OS on to your Raspberry Pi](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up)
- [Enable the I2C interface in the raspi-config](https://www.raspberrypi.org/documentation/configuration/raspi-config.md)
- Download the driver for the [Sensor Board](https://github.com/Meapy011/Ras-Sensirion/tree/main) and extract the `.zip` on your Raspberry Pi
- Compile the driver
    1. Open a [terminal](https://www.raspberrypi.org/documentation/usage/terminal/?)
    2. Navigate to the driver directory. E.g. `cd ~/Ras-sensirion/Sensor-examble`
    3. Run the `./Build` script to check for updates, make sure Build essential tools are installed and compile the driver

       Output:
       ```
       
       ```
- Test your connected sensor
    - Run `./test-sensors` in the same directory you used to
      compile the driver.


## Resources:

- [Sensirion Site](https://sensirion.com/)
- [Sensirion Github](https://github.com/Sensirion)
- [Sen44 Docs](https://github.com/Sensirion/raspberry-pi-uart-sen44)
- [SCD30 Docs](https://github.com/Sensirion/raspberry-pi-i2c-scd30)
- [SFA30 Docs](https://github.com/Sensirion/raspberry-pi-uart-sfa3x)
- [SEN66 Docs](https://github.com/Sensirion/raspberry-pi-i2c-sen66)

