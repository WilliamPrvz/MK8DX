#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include "spi_comm.h"
#include <fat.h>
#include <audio/play_sound_file.h>
#include <audio/audio_thread.h>
#include "sensors/proximity.h"
#include "loop_control.h"

#include <leds.h>
#include <sdio.h>

#include <pi_regulator.h>
#include <process_image.h>
#include <character_selection.h>

#include <button.h>


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

bool has_been_pressed = false;


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


    //inits the microSD card
    sdio_start();

    dac_start();
    playSoundFileStart();

    //starts the camera
    dcmi_start();
	po8030_start();
	//inits the motors
	motors_init();


	//starts the threads for ir sensor
	messagebus_init(&bus, &bus_lock, &bus_condvar);
	proximity_start();
	calibrate_ir();

	//stars the threads for the pi regulator and the processing of the image

	process_image_start();



	//stars the threads for the character selection
	character_selection_start();

    /* Infinite loop. */
    while (1) {

    	if (button_is_pressed() && !has_been_pressed){

    		has_been_pressed = true;
        	pi_regulator_start();
        	loop_control_start();
    	}
    	//waits 1 second
        chThdSleepMilliseconds(100);

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
