/*
 * character_selection.c
 *
 *  Created on: 27 avr. 2021
 *      Author: prove
 */


#include "hal.h"

#include <leds.h>
#include <selector.h>
#include <character_selection.h>
#include <main.h>
#include "usbcfg.h"
#include "spi_comm.h"
#include <chprintf.h>
#include <fat.h>
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include <sdio.h>




static THD_WORKING_AREA(waCharacterSelection, 256);
static THD_FUNCTION(CharacterSelection, arg) {



	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;

    sdio_connect();

    int old_status = 0;
    
    
		
//	int k = 0;
	while (1){
		
		int status = get_selector();
		time = chVTGetSystemTime();

		if (status != old_status) {
			
			old_status = status;

 //Faudra faire attention aux Magic numbers avec les RGB je pense
			switch(status) {


						case 0: // Mario



							playSoundFile("Son_MK8DX/itsame_mario.wav", SF_SIMPLE_PLAY);

							set_rgb_led(LED2, 100,0,0);
							set_rgb_led(LED4, 100,0,0);
							set_rgb_led(LED6, 0,0,100);
							set_rgb_led(LED8, 0,0,100);

						break;

						case 1: // Luigi
						
							playSoundFile("Son_MK8DX/luigi_ohyeahluigi.wav", SF_SIMPLE_PLAY);
							
							set_rgb_led(LED2, 9,87,17);
							set_rgb_led(LED4, 9,87,17);
							set_rgb_led(LED6, 8,13,100);
							set_rgb_led(LED8, 8,13,100);

						break;

						case 2: // Peach
						
							playSoundFile("Son_MK8DX/peach_peach.wav", SF_SIMPLE_PLAY);
						
							set_rgb_led(LED2, 94,57,74);
							set_rgb_led(LED4, 94,57,74);
							set_rgb_led(LED6, 33,73,82);
							set_rgb_led(LED8, 33,73,82);

						break;

						case 3: // Daisy
						
							playSoundFile("Son_MK8DX/daisy_yeahdaisy.wav", SF_SIMPLE_PLAY);
						
							set_rgb_led(LED2, 100,100,20);
							set_rgb_led(LED4, 100,100,20);
							set_rgb_led(LED6, 80,40,0);
							set_rgb_led(LED8, 80,40,0);

						break;

						case 4: // Toad
						
							playSoundFile("Son_MK8DX/toad.wav", SF_SIMPLE_PLAY);
							set_rgb_led(LED2, 95,14,6);
							set_rgb_led(LED4, 95,14,6);
							set_rgb_led(LED6, 97,97,97);
							set_rgb_led(LED8, 97,97,97);

						break;

						case 5: // Yoshi
						
							playSoundFile("Son_MK8DX/yoshi.wav", SF_SIMPLE_PLAY);
							
							set_rgb_led(LED2, 1,86,0);
							set_rgb_led(LED4, 1,86,0);
							set_rgb_led(LED6, 96,96,96);
							set_rgb_led(LED8, 96,96,96);

						break;

						case 6: // Wario
						
							playSoundFile("Son_MK8DX/wario_wariotime.wav", SF_SIMPLE_PLAY);
							
							set_rgb_led(LED2, 94,100,8);
							set_rgb_led(LED4, 94,100,8);
							set_rgb_led(LED6, 61,13,98);
							set_rgb_led(LED8, 61,13,98);

						break;

						case 7: // Waluigi
						
							playSoundFile("Son_MK8DX/waluigi_waluigi.wav", SF_SIMPLE_PLAY);	
							
							set_rgb_led(LED2, 61,13,98);
							set_rgb_led(LED4, 61,13,98);
							set_rgb_led(LED6, 0,0,25);
							set_rgb_led(LED8, 0,0,25);

						break;

		}


		}
		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}



}
void character_selection_start(void){
	chThdCreateStatic(waCharacterSelection, sizeof(waCharacterSelection), NORMALPRIO, CharacterSelection, NULL);
}

