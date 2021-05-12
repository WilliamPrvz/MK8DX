/*
 * loop_control.h
 *
 *  Created on: 29 avr. 2021
 *      
 */

#ifndef LOOP_CONTROL_H_
#define LOOP_CONTROL_H_

#define PROXIMITY_THRESHOLD 	100 	//corresponds to around 5cm
#define MAX_LAPS				4 		// number of laps to complete the circuit, including the beginning of the circuit (3+1)


void loop_control_start(void);
bool get_circuit_completed(void);


#endif /* LOOP_CONTROL_H_ */
