//pi_regulator.c of TP4 camreg
//Modified to implement the line following

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
#include <items_gestion.h>
#include <button.h>
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>


//simple PI regulator implementation
int16_t pi_regulator(float distance, float goal){

	float error = 0;
	float speed = 0;

	static float sum_error = 0;

	error = distance - goal;

	//disables the PI regulator if the error is to small
	//this avoids to always move as we cannot exactly be where we want and 
	//the camera is a bit noisy
	if(fabs(error) < ERROR_THRESHOLD){
		return 0;
	}

	sum_error += error;

	//we set a maximum and a minimum for the sum to avoid an uncontrolled growth
	if(sum_error > MAX_SUM_ERROR){
		sum_error = MAX_SUM_ERROR;
	}else if(sum_error < -MAX_SUM_ERROR){
		sum_error = -MAX_SUM_ERROR;
	}

	speed = KP * error + KI * sum_error;

    return (int16_t)speed;
}

static THD_WORKING_AREA(waPiRegulator, 256);
static THD_FUNCTION(PiRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    
	
	int16_t speed = 300;			//MAGIC NUMBER
	int16_t speed_correction = 0;



	while(1){

		time = chVTGetSystemTime();


		if(!get_circuit_completed()) {

			speed = 300;
			//dealing with items
			

			//chprintf((BaseSequentialStream *)&SDU1, "mushroom ?= %d\r\n\n", get_mushroom());

			if(get_mushroom()) {
				
				speed = 800;		// MAGIC NUMBER : MUSHROOM_SPEED
				
				//chThdSleepMilliseconds(2000);		//mushroom duration
			
				//speed = 300;			//REMPLACER PLUS TARD PAR LE DEFINE USUAL_SPEED
				
				//chprintf((BaseSequentialStream *)&SDU1, "if mushroom speed= %d\r\n\n", speed);

			}
			
			//if the robot encounters a shell then the PI regulator isn't taken into account
			
			if (!get_shell()) {

				//chprintf((BaseSequentialStream *)&SDU1, "speed= %d\r\n\n", speed);

			
		//        //computes the speed to give to the motors
		//        //distance_cm is modified by the image processing thread
		//        speed = pi_regulator(get_distance_cm(), GOAL_DISTANCE);

				//computes a correction factor to let the robot rotate to be in front of the line
				speed_correction = (get_line_position() - (IMAGE_BUFFER_SIZE/2));

				//if the line is nearly in front of the camera, don't rotate
				if(abs(speed_correction) < ROTATION_THRESHOLD){
					speed_correction = 0;
				}

				//applies the speed from the PI regulator and the correction for the rotation
				right_motor_set_speed(speed - ROTATION_COEFF * speed_correction);
				left_motor_set_speed(speed + ROTATION_COEFF * speed_correction);
			}

			
			if(get_shell()) {
				
				left_motor_set_pos(0);
				right_motor_set_pos(0);	
								
				while(left_motor_get_pos()<PERIMETER_EPUCK*(1000/13)) {
					left_motor_set_speed(500);
					right_motor_set_speed(-500);
					//chprintf((BaseSequentialStream *)&SDU1, "motor position = %3d\r\n\n", left_motor_get_pos());
				}
				left_motor_set_speed(speed);
				right_motor_set_speed(speed);

				chThdSleepMilliseconds(1000);

			}			
		}

		else {
			
			playSoundFile("Son_MK8DX/mario_finish.wav", SF_SIMPLE_PLAY);
			//chThdSleepMilliseconds(3000);

			while (speed > 100){
				
				right_motor_set_speed(speed);
				left_motor_set_speed(speed);
				speed = speed - 10;
				chThdSleepMilliseconds(100);
				
			}
			right_motor_set_speed(0);
			left_motor_set_speed(0);



		}

		//100Hz
		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}
}

void pi_regulator_start(void){
	chThdCreateStatic(waPiRegulator, sizeof(waPiRegulator), NORMALPRIO, PiRegulator, NULL);
}


//void pi_regulator_suspend(void){
//	chThdSuspendS();
//}
//
//void pi_regulator_resume(void){
//	chThdResume();
//}


//void pi_regulator_stop(void){
//chThdTerminate(PiRegulator);
//	chThdWait(PiRegulator);
//	PiRegulator = NULL;
//}
