#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <pi_regulator.h>
#include <process_image.h>
#include <loop_control.h>
#include <button.h>
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>


static THD_WORKING_AREA(waItemsGestion, 256);
static THD_FUNCTION(LoopItemsGestion, arg) {
	
	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	
	while(1) {
		
		time = chVTGetSystemTime();
		
		if(get_image_red_moy() > RED_THRESHOLD) {
			
			right_motor_set_speed(400);			//MAGIC NUMBER A MODIF
			left_motor_set_speed(400);
			
			chThdSleepMilliseconds(2000);		//mushroom duration
			
			right_motor_set_speed(300);			//METTRE SPEED EN VARIABLE GLOBALE ?
			left_motor_set_speed(300);		
		}
		
		
		if(get_image_green_moy > GREEN_THRESHOLD) {
			
			left_motor_set_speed(0);
			
			chThdSleepMilliseconds(2000);
			
			left_motor_set_speed(300);
		}
		
		
	}
	
	
}

void items_gestion_start(void){
	chThdCreateStatic(waItemsGestion, sizeof(waItemsGestion), NORMALPRIO, ItemsGestion, NULL);
}
