/*
 * loop_control.c
 *
 *  Created on: 29 avr. 2021
 *      Author: prove
 */

#include "hal.h"
#include <loop_control.h>
#include <sensors/proximity.h>
#include <main.h>
#include <chprintf.h>
#include "usbcfg.h"

static bool circuit_completed = false; // becomes true once 3 loops are completed

static THD_WORKING_AREA(waLoopControl, 256);
static THD_FUNCTION(LoopControl, arg) {

	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	
	unsigned int loop_number = 0;

	while (1){
		time = chVTGetSystemTime();
		
		if ((get_prox(IR_RIGHT) > PROXIMITY_THRESHOLD)&&(get_prox(IR_LEFT) > PROXIMITY_THRESHOLD)){
			
			++loop_number;
			chThdSleepMilliseconds(1000); // makes sure that it increments loop_number only once

		}

		if (loop_number >= 3){
		
			circuit_completed = true;

		}

	chprintf((BaseSequentialStream *)&SDU1, "Right : %4d, \r\n", get_prox(IR_RIGHT));
	chprintf((BaseSequentialStream *)&SDU1, "Left : %4d,\r\n", get_prox(IR_LEFT));
	
	chprintf((BaseSequentialStream *)&SDU1, "loop_number : %u, \r\n", loop_number);
	chprintf((BaseSequentialStream *)&SDU1, "circuit_completed : %d, \r\n", circuit_completed);



		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}


}


void loop_control_start(void){
	chThdCreateStatic(waLoopControl, sizeof(waLoopControl), NORMALPRIO, LoopControl, NULL);
}

bool get_circuit_completed(void) {
	return circuit_completed;
}
