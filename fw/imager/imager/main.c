/*
 * imager.c
 *
 * Created: 18.02.2024 15:10:22
 * Author : Minty
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "power.h"
#include "led.h"
#include "accelerometer.h"


int main(void) {
    init_power();
	init_led();
	init_accelerometer();
	// delay so leds won't take initial voltage spike from DDRA setup as an extra bit
	_delay_us(80);
	line[0].red = 255; line[0].green = 255; line[0].blue = 255;
	line[1].red = 127; line[1].green = 127; line[1].blue = 127;
	line[2].red = 31;  line[2].green = 31;  line[2].blue = 31;
	line[3].red = 1;   line[3].green = 1;   line[3].blue = 1;
	line[4].red = 63;  line[4].green = 255; line[4].blue = 127;
	line[5].red = 31;  line[5].green = 255; line[5].blue = 127;
	line[6].red = 255; line[6].green = 31;  line[6].blue = 127;
	line[7].red = 255; line[7].green = 63;  line[7].blue = 192;
	refresh_leds();
	sei();
    while (1) {
		sleep_mode();
    }
}

