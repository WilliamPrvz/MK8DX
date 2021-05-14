/*
 * lap_control.c
 *
 *  Detects if the robot completes a lap.
 * 	If it completes 3 then the circuit is over.
 *      
 */

#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include "hal.h"
#include <sdio.h>
#include <sensors/proximity.h>
#include "spi_comm.h"
#include "usbcfg.h"

#include <lap_control.h>


static bool circuit_completed = false;		// becomes true once 3 laps are completed

static THD_WORKING_AREA(waLapControl, 256);
static THD_FUNCTION(LapControl, arg) {

	chRegSetThreadName(__FUNCTION__);
	
	(void)arg;

	systime_t time;
	
	unsigned int lap_counter = 0;

	while(1){
		
		time = chVTGetSystemTime();
		
		// checks if the robot goes through the doors of the end of a lap
		if ((get_prox(IR_RIGHT) > PROXIMITY_THRESHOLD) && (get_prox(IR_LEFT) > PROXIMITY_THRESHOLD)){
			
			lap_counter++;
			
			chThdSleepMilliseconds(2000); // makes sure that it increments lap_number only once

		}

		if (lap_counter == MAX_LAPS){		//circuit is over
		
			circuit_completed = true;
		}

		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}

}


void lap_control_start(void){
	chThdCreateStatic(waLapControl, sizeof(waLapControl), NORMALPRIO, LapControl, NULL);
}

bool get_circuit_completed(void){
	return circuit_completed;
}
