#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <pi_regulator.h>
#include <process_image.h>
#include <items_gestion.h>
#include <loop_control.h>
#include <button.h>
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>

static bool mushroom = false;
static bool shell = false;

static THD_WORKING_AREA(waItemsGestion, 1024);
static THD_FUNCTION(ItemsGestion, arg) {
	
	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	
	while(1) {
		
		time = chVTGetSystemTime();
		
		//chprintf((BaseSequentialStream *)&SDU1, "Wow=%3d\r\n\n", get_image_red_moy());

		if((get_image_red_moy()>RED_THRESHOLD)&&(get_image_green_moy()<GREEN_THRESHOLD)) {


			mushroom = true;
			chThdSleepMilliseconds(2000);
			mushroom = false;
		}
		
		
		if((get_image_green_moy()>GREEN_THRESHOLD)&&(get_image_red_moy()<RED_THRESHOLD)) {
			
			shell = true;
			chThdSleepMilliseconds(100);
			shell = false;
			 
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
