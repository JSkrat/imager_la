/*
 * power.h
 *
 * Created: 18.02.2024 16:29:51
 *  Author: Minty
 */


#ifndef POWER_H_
#define POWER_H_
#include <avr/io.h>
#include <avr/sleep.h>

extern inline void init_power() {
	set_sleep_mode(SLEEP_MODE_IDLE);
	// comparator
	ACSR = (1 << ACD);
	// shut down unused components
	// ADC, timer 1
	PRR = (1 << PRADC) | (1 << PRTIM1);
}



#endif /* POWER_H_ */