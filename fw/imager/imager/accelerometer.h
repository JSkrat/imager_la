/*
 * accelerometer.h
 *
 * Created: 21.02.2024 22:10:46
 *  Author: Minty
 */


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <avr/io.h>
#include <util/delay.h>
#include "hw.h"

extern inline void init_accelerometer() {
	ACC_DDR &= ~((1 << ACC_INT1));
	ACC_DDR |= (1 << ACC_CS) | (1 << PORTA4) | (1 << PORTA6);
	/* CSB pin is made high for selecting SPI protocol
     * Note: CSB has to see rising after power up, to switch to SPI protocol */
	ACC_PORT &= ~(1 << ACC_CS);
	_delay_ms(10);
	// set CS inactive
	ACC_PORT |= (1 << ACC_CS);
	// init SPI
	// three-wire mode, clock source is external, positive edge, software clock strobe
	USICR = (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0) | (1 << USICLK);
}




#endif /* ACCELEROMETER_H_ */