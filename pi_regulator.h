#ifndef PI_REGULATOR_H
#define PI_REGULATOR_H

#define PI					3.1415926536f
#define WHEEL_DISTANCE		5.35f
#define NSTEP_ONE_TURN		1000		// number of steps for 1 turn of the motor
#define WHEEL_PERIMETER		13			//  [cm]
#define PERIMETER_EPUCK		(PI*WHEEL_DISTANCE)

#define NORMAL_SPEED		500			// robot speed deuring the circuit if no item is detected
#define	MUSHROOM_SPEED		1000		// robot speed when encountering a mushroom
#define ROTATING_SPEED		800			// robot rotating speed when encountering a shell
#define ENDING_SPEED		100			// aiming speed when slowly decreasing at the end of the circuit


void pi_regulator_start(void);

#endif /* PI_REGULATOR_H */
