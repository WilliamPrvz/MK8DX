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


	// Faudra faire attention aux Magic numbers avec les RGB je pense
		switch(get_selector()) {
						case 0: // Mario
							set_rgb_led(LED2, 100,0,0);
							set_rgb_led(LED4, 100,0,0);
							set_rgb_led(LED6, 0,100,100);
							set_rgb_led(LED8, 0,100,100);

						break;

						case 1: // Luigi
							set_rgb_led(LED2, 9,87,17);
							set_rgb_led(LED4, 9,87,17);
							set_rgb_led(LED6, 8,13,100);
							set_rgb_led(LED8, 8,13,100);

						break;

						case 2: // Peach
							set_rgb_led(LED2, 94,57,74);
							set_rgb_led(LED4, 94,57,74);
							set_rgb_led(LED6, 33,73,82);
							set_rgb_led(LED8, 33,73,82);

						break;
						
						case 3: // Daisy
							set_rgb_led(LED2, 100,100,20);
							set_rgb_led(LED4, 100,100,20);
							set_rgb_led(LED6, 80,40,0);
							set_rgb_led(LED8, 80,40,0);

						break;

						case 4: // Toad
							set_rgb_led(LED2, 95,14,6);
							set_rgb_led(LED4, 95,14,6);
							set_rgb_led(LED6, 97,97,97);
							set_rgb_led(LED8, 97,97,97);

						break;
						
						case 5: // Yoshi
							set_rgb_led(LED2, 1,86,0);
							set_rgb_led(LED4, 1,86,0);
							set_rgb_led(LED6, 96,96,96);
							set_rgb_led(LED8, 96,96,96);

						break;

						case 6: // Wario
							set_rgb_led(LED2, 94,100,8);
							set_rgb_led(LED4, 94,100,8);
							set_rgb_led(LED6, 61,13,98);
							set_rgb_led(LED8, 61,13,98);

						break;
						
						case 7: // Waluigi
							set_rgb_led(LED2, 61,13,98);
							set_rgb_led(LED4, 61,13,98);
							set_rgb_led(LED6, 0,0,25);
							set_rgb_led(LED8, 0,0,25);

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
