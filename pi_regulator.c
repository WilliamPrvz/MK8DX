//pi_regulator.c of TP4 camreg
//Modified to implement the line following

/*
 * pi_regulator.c
 *
 *  Based on the file from TP4 CamReg.
 *  Implements a PI regulator so that the robot keeps following the 
 * 	black line at constant speed.
 * 	Also modifies the behaviour of motors when items are detected.   
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
#include <lap_control.h>
#include <main.h>
#include <pi_regulator.h>
#include <process_image.h>


bool has_stopped = false;		// makes sure the robot stops and plays the final melody only once

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
    
	int16_t speed = NORMAL_SPEED;
	int16_t speed_correction = 0;

	while(1){

		time = chVTGetSystemTime();


		if(!get_circuit_completed()){

			speed = NORMAL_SPEED;
			
			
			//dealing with items : 
			//	coming across a mushroom gives the robot a boost of speed during a defined time (items_gestion)
			//	coming across a green shell makes the robot stop and rotate 360 degrees before moving forward again

			if(get_mushroom()){
				
				speed = MUSHROOM_SPEED;	

			}
			
			//if the robot encounters a shell then the PI regulator isn't taken into account
			if (!get_shell()){

			//computes a correction factor to let the robot rotate to be in front of the line
			speed_correction = pi_regulator(get_line_position(), IMAGE_BUFFER_SIZE/2);
			
			//applies the speed from the PI regulator and the correction for the rotation
			right_motor_set_speed(speed -  speed_correction);
			left_motor_set_speed(speed + speed_correction);
			
			}

			if(get_shell()){
				
				left_motor_set_pos(0);
				right_motor_set_pos(0);	
				
				// computing a rotation of 360 degrees 				
				while(left_motor_get_pos()<= PERIMETER_EPUCK*NSTEP_ONE_TURN/WHEEL_PERIMETER){
					
					left_motor_set_speed(ROTATING_SPEED);
					right_motor_set_speed(-ROTATING_SPEED);
				}

				left_motor_set_speed(speed);
				right_motor_set_speed(speed);

				chThdSleepMilliseconds(1000);

			}
		}

		if ((has_stopped==false) && get_circuit_completed()){
			
			playSoundFile("Son_MK8DX/mario_finish.wav", SF_WAIT_AND_CHANGE);
			
			//progressively decreasing the speed of the robot once the circuit is finished
			while (speed > ENDING_SPEED){
				
				right_motor_set_speed(speed);
				left_motor_set_speed(speed);
				speed = speed - 10;
				chThdSleepMilliseconds(100);
				
			}
			right_motor_set_speed(0);
			left_motor_set_speed(0);

			has_stopped = true;
		}

		//100Hz
		chThdSleepUntilWindowed(time, time + MS2ST(10));

	}

}



void pi_regulator_start(void){
	chThdCreateStatic(waPiRegulator, sizeof(waPiRegulator), NORMALPRIO, PiRegulator, NULL);
}
