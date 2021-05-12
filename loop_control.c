/*
 * loop_control.c
 *
 *  Detects if the robot completes a lap.
 * 	If it completes 3 then the circuit is over.
 *      
 */

#include "hal.h"
#include <loop_control.h>
#include <sensors/proximity.h>
#include <main.h>
#include <chprintf.h>
#include "usbcfg.h"
#include "spi_comm.h"
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include <sdio.h>

static bool circuit_completed = false;		// becomes true once 3 laps are completed

static THD_WORKING_AREA(waLoopControl, 256);
static THD_FUNCTION(LoopControl, arg) {

	chRegSetThreadName(__FUNCTION__);
	
	(void)arg;

	systime_t time;
	
	unsigned int loop_number = 0;

	while(1){
		
		time = chVTGetSystemTime();
		
		// checks if the robot goes through the doors of the end of a lap
		if ((get_prox(IR_RIGHT) > PROXIMITY_THRESHOLD) && (get_prox(IR_LEFT) > PROXIMITY_THRESHOLD)){
			
			loop_number++;
			
			chThdSleepMilliseconds(2000); // makes sure that it increments loop_number only once

		}

		if (loop_number == MAX_LAPS){		//circuit is over
		
			circuit_completed = true;
		}

		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}

}


void loop_control_start(void){
	chThdCreateStatic(waLoopControl, sizeof(waLoopControl), NORMALPRIO, LoopControl, NULL);
}

bool get_circuit_completed(void){
	return circuit_completed;
}
