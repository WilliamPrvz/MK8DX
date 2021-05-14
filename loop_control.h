#ifndef LAP_CONTROL_H_
#define LAP_CONTROL_H_

#define IR_LEFT					5
#define IR_RIGHT				2
#define PROXIMITY_THRESHOLD 	100 	// corresponds to around 5cm
#define MAX_LAPS				4 		// number of laps to complete the circuit, including the beginning of the circuit (3+1)


void lap_control_start(void);
bool get_circuit_completed(void);


#endif /* LAP_CONTROL_H_ */
