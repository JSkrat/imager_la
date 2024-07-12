/*
 * image.c
 *
 * Created: 17.03.2024 2:03:07
 *  Author: Minty
 */

#include "image.h"
#include "led.h"
#include "accelerometer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_image() {
	// CTC operation mode (count from 0 to OCR0A
	TCCR0A = (1 << WGM01) | (0 << WGM00);
	// clk_io/1024 (T0 ovf freq is 30Hz, if count to 78, 100Hz)
	TCCR0B = (0 << WGM02) | (1 << CS02) | (0 << CS01) | (1 << CS00);
	OCR0A = 78;
	// enable timer0 overflow interrupt
	TIMSK0 |= (1 << OCIE0A);
}

ISR(TIM0_COMPA_vect) {
	uint16_t vec[3] = {
		read_x(),
		read_y(),
		read_z(),
	};
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 3; ++j) {
			#define STEP (65536/8)
			int32_t x = vec[j];
			x -= i*STEP;
			if (0 > x) x = 0;
			if (255 < x) x = 255;
			switch (j) {
				case 0: { line[i].red = (uint8_t) x; break; }
				case 1: { line[i].green = (uint8_t) x; break; }
				case 2: { line[i].blue = (uint8_t) x; break; }
			}
			#undef STEP
		}
	}
	refresh_leds();
}
