/*
 * led.h
 *
 * Created: 18.02.2024 19:20:31
 *  Author: Minty
 */ 


#ifndef LED_H_
#define LED_H_
#include "hw.h"

inline void init_led() {
	LED_DDR |= (1 << LED_PIN);
}

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} sLedColor;
#define LED_NUMBER 8
extern sLedColor line[LED_NUMBER];
extern void refresh_leds();


#endif /* LED_H_ */