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
#include "spi_comm.h"
#include <chprintf.h>




static THD_WORKING_AREA(waCharacterSelection, 256);
static THD_FUNCTION(CharacterSelection, arg) {


	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	while (1){
		time = chVTGetSystemTime();



		switch(get_selector()) {
						case 0: // Mario
							set_rgb_led(LED2, 8,70,158);
							set_rgb_led(LED4, 8,70,158);
							set_rgb_led(LED6, 194,53,45);
							set_rgb_led(LED8, 194,53,45);

						break;

						case 1: // Luigi
							set_rgb_led(LED2, 24,222,44);
							set_rgb_led(LED4, 24,222,44);
							set_rgb_led(LED6, 21,35,255);
							set_rgb_led(LED8, 21,35,255);

						break;


		}



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

