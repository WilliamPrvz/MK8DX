#ifndef ITEMS_GESTION_H_
#define ITEMS_GESTION_H_

// high and low thresholds for each color to make sure it's not another color or white (which is everywhere on the circuit)
#define RED_HIGH_THRESHOLD 		100
#define RED_LOW_THRESHOLD 		50
#define GREEN_HIGH_THRESHOLD 	80
#define GREEN_LOW_THRESHOLD 	50

#define MUSHROOM_DURATION		2000 // 2 seconds
#define SHELL_IGNORING_TIME		3000 // 3 seconds

void items_gestion_start(void);
bool get_mushroom(void);
bool get_shell(void);


#endif /* ITEMS_GESTION_H_ */
