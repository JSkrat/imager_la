/*
 * led.c
 *
 * Created: 18.02.2024 19:20:22
 *  Author: Minty
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
void led_send_byte(uint8_t b);


sLedColor line[LED_NUMBER];

// should be called from interrupt routine under CLI conditions
void refresh_leds() {
	//cli();
	// delay between led_send_byte should not be more than 80us
	for (int i = 0; i < LED_NUMBER; ++i) {
		led_send_byte(line[i].green);
		led_send_byte(line[i].red);
		led_send_byte(line[i].blue);
	}
	//sei();
}
