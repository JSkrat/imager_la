/*
 * accelerometer.h
 *
 * Created: 21.02.2024 22:10:46
 *  Author: Minty
 */ 


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <avr/io.h>
#include "hw.h"

extern inline void init_accelerometer() {
	ACC_DDR &= ~((1 << ACC_INT1));
	ACC_DDR |= (1 << ACC_CS);
	// set CS inactive
	ACC_PORT |= (1 << ACC_CS);
}




#endif /* ACCELEROMETER_H_ */