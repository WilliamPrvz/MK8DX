/*
 * character_selection.c
 *
 *  Created on: 27 avr. 2021
 *      Author: prove
 */


#include "hal.h"

#include <leds.h>
#include <selector.h>
#include <character_selection.h>
#include <main.h>
#include "usbcfg.h"

#include <chprintf.h>




static THD_WORKING_AREA(waCharacterSelection, 256);
static THD_FUNCTION(CharacterSelection, arg) {


	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	while (1){
		time = chVTGetSystemTime();



//		switch(get_selector()) {
//						case 0: // Mario
//							set_rgb_led(LED2, 255, 0, 0);
//							set_rgb_led(LED4, 255, 0, 0);
//							set_rgb_led(LED6, 0, 0, 255);
//							set_rgb_led(LED8, 0, 0, 255);
//
//						break;
//
//						case 1: // Mario
//							set_rgb_led(LED2, 255, 0, 0);
//							set_rgb_led(LED4, 255, 0, 0);
//							set_rgb_led(LED6, 0, 0, 255);
//							set_rgb_led(LED8, 0, 0, 255);
//
//						break;
//
//
//		}

		set_rgb_led(0, 0, 0, 10);
		set_rgb_led(1, 0, 0, 10);
		set_rgb_led(2, 0, 0, 10);
		set_rgb_led(3, 0, 0, 10);
		chprintf((BaseSequentialStream *)&SDU1, "%4d,", get_selector());



		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}


//	if (character == 1){
//		set_rgb_led(LED2, 255, 0, 0);
//		set_rgb_led(LED4, 255, 0, 0);
//		set_rgb_led(LED6, 0, 0, 255);
//		set_rgb_led(LED8, 0, 0, 255);
//
//	};

	//100Hz
	//chThdSleepUntilWindowed(time, time + MS2ST(10));

}
void character_selection_start(void){
	chThdCreateStatic(waCharacterSelection, sizeof(waCharacterSelection), NORMALPRIO, CharacterSelection, NULL);
	chprintf((BaseSequentialStream *)&SDU1, "hello");
}

