#ifndef PI_REGULATOR_H
#define PI_REGULATOR_H

#define PI		3.1415926536f
#define WHEEL_DISTANCE		5.35f
#define PERIMETER_EPUCK		(PI*WHEEL_DISTANCE)

//start the PI regulator thread
void pi_regulator_start(void);
//void pi_regulator_suspend(void);
//void pi_regulator_resume(void);
//void pi_regulator_stop(void);


#endif /* PI_REGULATOR_H */
