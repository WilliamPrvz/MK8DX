/*
 * character_selection.h
 *
 *  Created on: 27 avr. 2021
 *     
 */

#ifndef CHARACTER_SELECTION_H_
#define CHARACTER_SELECTION_H_

enum mario_characters{		// Corresponding to selector position 0 to 7
	MARIO, 
	LUIGI, 
	PEACH, 
	DAISY, 
	TOAD, 
	YOSHI, 
	WARIO, 
	WALUIGI};

void character_selection_start(void);

#endif /* CHARACTER_SELECTION_H_ */
