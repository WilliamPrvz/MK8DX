/*
 * main.c
 *
 *  Based on the file from TP4 CamReg.
 *  Initializes all the threads and communications.
 *      
 */

#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include <button.h>
#include <camera/po8030.h>
#include "ch.h"
#include <fat.h>
#include "hal.h"
#include <leds.h>
#include <math.h>
#include "memory_protection.h"
#include <motors.h>
#include <sdio.h>
#include "sensors/proximity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_comm.h"
#include <usbcfg.h>

#include <character_selection.h>
#include <items_gestion.h>
#include <lap_control.h>
#include <main.h>
#include <pi_regulator.h>
#include <process_image.h>



messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

static bool has_been_pressed = false;


void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    
    //start the USB communication
    usb_start();
    spi_comm_start();

    //clear the leds
    clear_leds();

    //starts the microSD card
    sdio_start();

	//starts the audio
    dac_start();
    playSoundFileStart();

    //starts the camera
    dcmi_start();
	po8030_start();

	po8030_set_awb(FALSE);
	po8030_set_rgb_gain(0X8D, 0x60, 0x5D);
	
	//inits the motors
	motors_init();

	//starts the IR sensors
	messagebus_init(&bus, &bus_lock, &bus_condvar);
	proximity_start();
	calibrate_ir();

	//starts the processing of images and the gestion of items
	process_image_start();
	items_gestion_start();

	//starts the selection of characters
	character_selection_start();

    /* Infinite loop. */
    while(1){

    	if (button_is_pressed() && !has_been_pressed){

    		has_been_pressed = true;
        	pi_regulator_start();
        	lap_control_start();
    	}
    	//waits 0.1 second
        chThdSleepMilliseconds(100);

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void){
    chSysHalt("Stack smashing detected");
}
