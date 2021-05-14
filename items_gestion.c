/*
 * items_gestion.c
 *
 *  Dealing with items. First checking from process_image.c if a red 
 *  or green color is detected, then sending the informations to
 * 	to pi_regulator.c.
 *      
 */

#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include <button.h>
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <motors.h>
#include <usbcfg.h>

#include <items_gestion.h>
#include <process_image.h>


static bool mushroom = false;		// becomes true if a mushroom is detected
static bool shell = false;			// // becomes true if a green_shell is detected

static THD_WORKING_AREA(waItemsGestion, 1024);
static THD_FUNCTION(ItemsGestion, arg) {
	
	chRegSetThreadName(__FUNCTION__);
	
	(void)arg;

	systime_t time;
	
	while(1){
		
		time = chVTGetSystemTime();
		
		// red is detected -> mushroom
		if((get_image_red_moy()>RED_HIGH_THRESHOLD)&&(get_image_green_moy()<GREEN_LOW_THRESHOLD)) {

			mushroom = true;
			chThdSleepMilliseconds(MUSHROOM_DURATION);
			mushroom = false;
		}

		// green is detected -> shell
		if((get_image_green_moy()>GREEN_HIGH_THRESHOLD)&&(get_image_red_moy()<RED_LOW_THRESHOLD)) {

			shell = true;
			chThdSleepMilliseconds(100);
			shell = false;
			chThdSleepMilliseconds(SHELL_IGNORING_TIME);	// avoiding the robot to detect the same green sticker once again and turn around infinitely

		}
		
		//100Hz
		chThdSleepUntilWindowed(time, time + MS2ST(10));

	}
	
	
}

void items_gestion_start(void){
	chThdCreateStatic(waItemsGestion, sizeof(waItemsGestion), NORMALPRIO, ItemsGestion, NULL);
}

bool get_mushroom(void) {
	return mushroom;
}

bool get_shell(void) {
	return shell;
}
