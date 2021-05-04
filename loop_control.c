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

static unsigned int loop_number = 0;

static THD_WORKING_AREA(waLoopControl, 256);
static THD_FUNCTION(LoopControl, arg) {

	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;

	while (1){
		time = chVTGetSystemTime();

//		if ((get_prox(IR_RIGHT)<10)&&(get_prox(IR_LEFT)<10)){
//			loopNb ++;
//			chThdSleepMilliseconds(1000);
//
//		}
//
//		if (loopNb >= 3){
//
//		}

	chprintf((BaseSequentialStream *)&SDU1, "Right : %4d,", get_prox(IR_RIGHT));
	chprintf((BaseSequentialStream *)&SDU1, "Left : %4d,", get_prox(IR_LEFT));


		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}


}


void loop_control_start(void){
	chThdCreateStatic(waLoopControl, sizeof(waLoopControl), NORMALPRIO, LoopControl, NULL);
}
