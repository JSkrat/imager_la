/*
 * accelerometer.h
 *
 * Created: 21.02.2024 22:10:46
 *  Author: Minty
 */


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


extern void init_accelerometer();

int8_t read(uint8_t reg_addr, uint8_t *data, uint8_t len);


#endif /* ACCELEROMETER_H_ */