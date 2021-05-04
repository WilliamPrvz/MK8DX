/*
 * loop_control.h
 *
 *  Created on: 29 avr. 2021
 *      Author: prove
 */

#ifndef LOOP_CONTROL_H_
#define LOOP_CONTROL_H_

#define PROXIMITY_THRESHOLD 	100 //corresponds to around 5cm
#define MAX_LOOPS				3 // number of loops to complete the circuit


void loop_control_start(void);
bool get_circuit_completed(void);


#endif /* LOOP_CONTROL_H_ */
