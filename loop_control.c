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
#include "spi_comm.h"
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include <sdio.h>

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

		if (loop_number == MAX_LOOPS){
		
			circuit_completed = true;
			playSoundFile("Son_MK8DX/mario_finish.wav", SF_FORCE_CHANGE);
			
		}

		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}


}


void loop_control_start(void){
	chThdCreateStatic(waLoopControl, sizeof(waLoopControl), NORMALPRIO, LoopControl, NULL);
}

bool get_circuit_completed(void) {
	return circuit_completed;
}
