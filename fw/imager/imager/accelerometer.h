/*
 * accelerometer.h
 *
 * Created: 21.02.2024 22:10:46
 *  Author: Minty
 */


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

/*!
 * @brief bmi160 sensor data structure which comprises of accel data
 */
struct bmi160_sensor_data
{
    /*! X-axis sensor data */
    int16_t x;

    /*! Y-axis sensor data */
    int16_t y;

    /*! Z-axis sensor data */
    int16_t z;

    /*! sensor time */
    uint32_t sensortime;
};

extern void init_accelerometer();

int8_t read(uint8_t reg_addr, uint8_t *data, uint8_t len);

int16_t read_x();
int16_t read_y();
int16_t read_z();

#endif /* ACCELEROMETER_H_ */