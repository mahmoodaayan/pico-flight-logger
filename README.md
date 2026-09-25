# Pico Flight Logger

A Raspberry Pi Pico 2 flight-data display prototype written in C++.

The project reads live atmospheric and inertial data over I²C, then displays the results on a 128×64 SSD1306 OLED screen. It is intended as a foundation for a future flight-data logging system.

> Current version: live display only. Data is not yet saved to an SD card or other storage.

## Features

* Temperature measurement in °C
* Atmospheric pressure measurement in kPa
* Three-axis acceleration in m/s²
* Three-axis angular velocity in degrees per second
* 128×64 OLED live data display
* 2 Hz screen update rate
* Sensor initialisation and communication error handling

## Hardware

| Component                   | Purpose                                    |
| --------------------------- | ------------------------------------------ |
| Raspberry Pi Pico 2         | Microcontroller                            |
| BMP280                      | Temperature and barometric pressure sensor |
| MPU6050                     | Three-axis accelerometer and gyroscope     |
| SSD1306 OLED                | 128×64 live data display                   |
| Breadboard and jumper wires | Prototyping connections                    |

## Wiring

All devices share the Pico's I²C0 bus.

| Signal | Raspberry Pi Pico 2 pin |
| ------ | ----------------------- |
| SDA    | GP0                     |
| SCL    | GP1                     |
| Power  | 3V3                     |
| Ground | GND                     |

The OLED, BMP280 and MPU6050 are connected in parallel to the same SDA and SCL lines.

## Display Output

The OLED displays four rows:

1. Temperature and pressure
2. X and Y acceleration
3. Z acceleration and X gyro rate
4. Y and Z gyro rate

Acceleration is converted from g to m/s² in the application:

```cpp
acceleration_ms2 = acceleration_g * 9.81f;
```

## Software

* C++17 application code
* Raspberry Pi Pico SDK
* CMake build system
* I²C communication at 400 kHz
* VS Code with the Raspberry Pi Pico extension

The project uses included SSD1306, BMP280 and MPU6050 driver libraries. Their original licence notices are retained in the relevant source files.

## Build and Flash

1. Open the project folder in VS Code.
2. Select the Raspberry Pi Pico 2 board in the Pico extension.
3. Build the project.
4. Connect the Pico by USB and use **Run Project (USB)**, or copy the generated `.uf2` file from the `build` folder to the Pico in BOOTSEL mode.

## Future Development

* Add SD-card data logging
* Record timestamps for each sensor sample
* Add maximum acceleration and altitude tracking
* Add sensor calibration routines
* Design a compact enclosure
* Integrate the logger into a wider student rocketry avionics system or a UAV avionics system 

## Author

Aayan Mahmood
Aerospace Engineering student
