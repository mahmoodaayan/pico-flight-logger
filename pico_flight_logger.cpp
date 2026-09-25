#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

extern "C" {
    #include "ssd1306.h"
    #include "driver_bmp280_basic.h"
    #include "driver_mpu6050_basic.h"
}

#define i2c_port i2c0
#define i2c_sda 0
#define i2c_scl 1

#define bmp280_address 0x76
#define bmp280_id_register 0xD0 
#define bmp280_pressure_register 0xF7
#define bmp280_temperature_register 0xFA
#define bmp280_measurement_register 0xF4
#define bmp280_temperature_calibration_register 0x88

#define oled_address 0x3C

#define mpu6050_address 0x68 
#define mpu6050_id_register 0x75 
#define mpu6050_ACCEL_OUT_X_H 0x3B
#define mpu6050_ACCEL_OUT_X_L 0x3C
#define mpu6050_ACCEL_OUT_Y_H 0x3D
#define mpu6050_ACCEL_OUT_Y_L 0x3E
#define mpu6050_ACCEL_OUT_z_H 0x3F
#define mpu6050_ACCEL_OUT_z_L 0x40
#define mpu_6050_GYRO_OUT_X_H 0x43
#define mpu_6050_GYRO_OUT_X_L 0x44
#define mpu_6050_GYRO_OUT_y_H 0x45
#define mpu_6050_GYRO_OUT_y_L 0x46
#define mpu_6050_GYRO_OUT_z_H 0x47
#define mpu_6050_GYRO_OUT_z_L 0x48



int main()
{
    stdio_init_all();

    i2c_init(i2c_port, 400000); // initialised the i2c port before communicatiing with devices 

    gpio_set_function(i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(i2c_scl, GPIO_FUNC_I2C);
    gpio_pull_up(i2c_sda);
    gpio_pull_up(i2c_scl);

    sleep_ms (500);

    ssd1306_t display; // initialised the OLED display 
    display.external_vcc = false;

    if(!ssd1306_init(&display, 128, 64, oled_address, i2c_port)) {
        printf("OLED library initialisation failed\n");

        while(true){
            tight_loop_contents();
        }
    }

sleep_ms(100);


uint8_t bmp_result = bmp280_basic_init(  // initialise the pressure temperature sensor 
BMP280_INTERFACE_IIC,
BMP280_ADDRESS_ADO_LOW
);

uint8_t mpu_result = mpu6050_basic_init( // initialise the gyro/accelerometer
    MPU6050_ADDRESS_AD0_LOW
);

if (bmp_result != 0 || mpu_result != 0) {
    printf("Sensor library initialisation failed\n");

    while (true) {
        tight_loop_contents();
    }
}

while(true) {

    float acceleration_g[3];
    float gyro_dps[3];

    bool mpu_ok = mpu6050_basic_read(acceleration_g, gyro_dps) == 0; // read teh acceleration in g and gyro in dps 

    float temperature_c = 0.0f;
    float pressure_pa = 0.0f;

    bool bmp_ok = bmp280_basic_read(&temperature_c, &pressure_pa) == 0;

    ssd1306_clear(&display);

    if (bmp_ok && mpu_ok){
        float pressure_kpa = pressure_pa / 1000.0f; // convert pascals to Kilo pascals
        float accel_x_ms2 = acceleration_g[0] * 9.81f; // convert acceleration from g to m/s^2 in all directions
        float accel_y_ms2 = acceleration_g[1] * 9.81f;
        float accel_z_ms2 = acceleration_g[2] * 9.81f;  

        char TemperaturePressure_line[24];
        char AxAy_line[24];
        char AzGx_line[24];
        char GyGz_line[24];

        snprintf(TemperaturePressure_line, sizeof(TemperaturePressure_line),  // shows the temperature and the pressure in the top row
                "T:%.1f P:%.2f", temperature_c, pressure_kpa);

        snprintf(AxAy_line, sizeof(AxAy_line),
                "Ax:%.1f Ay:%.1f", accel_x_ms2, accel_y_ms2); // shows the acceleration in the x and y in the second row

        snprintf(AzGx_line, sizeof(AzGx_line),
                "Az:%.1f Gx:%.1f", accel_z_ms2, gyro_dps[0]); // shows acceleration in the z and gyro in the x for the third row

        snprintf(GyGz_line, sizeof(GyGz_line),
                "Gy:%.1f Gz:%.1f", gyro_dps[1], gyro_dps[2]); // for the final row its showing gyro in y and z directions



        ssd1306_draw_string(&display, 0, 0, 1, TemperaturePressure_line);  // draws out each result on each row 
        ssd1306_draw_string(&display, 0, 16, 1, AxAy_line);
        ssd1306_draw_string(&display, 0, 32, 1, AzGx_line);
        ssd1306_draw_string(&display, 0, 48, 1, GyGz_line);
    }else{
        ssd1306_draw_string(&display, 0, 16, 1, "ERROR!!!");

    }

    ssd1306_show(&display); // transfers the complete display buffer to the physical OLED 


    sleep_ms(500); // refreshes the screen and results twice per second
}

}