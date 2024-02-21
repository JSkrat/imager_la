
/*
 * led_ll.s
 *
 * Created: 18.02.2024 20:43:26
 *  Author: Minty
 */ 
#define __ASSEMBLY__
#include <avr/io.h>
#include "hw.h"

// designed for 8MHz frequency
.global led_send_byte

led_send_byte:
#define data r24
    in r19, SREG - __SFR_OFFSET                     ; 1
    cli                                             ; 1

	// 8 bit to send
	ldi r18, 8										; 1
	cyc_bit_send:
		; next bit
		rol data									; 1
		brcs cbs_send_1								; 2
			cbs_send_0:
			// 0.2-0.4us 1
			// 0.8us 0
			// period 1.2us min
			sbi LED_PORT - __SFR_OFFSET, LED_PIN	; 2
			cbi LED_PORT - __SFR_OFFSET, LED_PIN	; 2
			nop
			rjmp cbs_next_bit						; 2

			cbs_send_1:
			// 0.58-1us 1
			// 0.2us 0
			// period 1.2us min
			sbi  LED_PORT - __SFR_OFFSET, LED_PIN	; 2
			nop
			nop
			nop
			nop
			cbi  LED_PORT - __SFR_OFFSET, LED_PIN	; 2
	cbs_next_bit:
	dec r18											; 1
	brne cyc_bit_send								; 2

    LED_PROC_EXIT:
    out SREG - __SFR_OFFSET, r19                    ; 1

    ret                                             ; 4

#undef __ASSEMBLY__