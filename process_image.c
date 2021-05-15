/*
 * process_image.c
 *
 *  Based on the file from TP4 CamReg.
 *  Allows the detection of the black line of the circuit and of the 
 *  colors for the different items.
 *      
 */

#include <camera/po8030.h>
#include "ch.h"
#include "hal.h"
#include <usbcfg.h>

#include <main.h>
#include <process_image.h>


static uint16_t line_position = IMAGE_BUFFER_SIZE/2;

static uint16_t image_red_ave = 0;
static uint16_t image_green_ave = 0;

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */
void extract_line(uint8_t *buffer){

	uint16_t i = 0, begin = 0, end = 0, width = 0;
	uint8_t stop = 0, wrong_line = 0, line_not_found = 0;
	uint32_t mean = 0;

	static uint16_t last_width = PXTOCM/GOAL_DISTANCE;

	//performs an average
	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;

	do{
		wrong_line = 0;
		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
		{ 
			//the slope must at least be WIDTH_SLOPE wide and is compared
		    //to the mean of the image
		    if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean)
		    {
		        begin = i;
		        stop = 1;
		    }
		    i++;
		}
		//if a begin was found, search for an end
		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
		{
		    stop = 0;
		    
		    while(stop == 0 && i < IMAGE_BUFFER_SIZE)
		    {
		        if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean)
		        {
		            end = i;
		            stop = 1;
		        }
		        i++;
		    }
		    //if an end was not found
		    if (i > IMAGE_BUFFER_SIZE || !end)
		    {
		        line_not_found = 1;
		    }
		}
		else//if no begin was found
		{
		    line_not_found = 1;
		}

		//if a line too small has been detected, continues the search
		if(!line_not_found && (end-begin) < MIN_LINE_WIDTH){
			i = end;
			begin = 0;
			end = 0;
			stop = 0;
			wrong_line = 1;
		}
	}
	while(wrong_line);

	if(line_not_found){
		begin = 0;
		end = 0;
		width = last_width;
	}
	else{
		last_width = width = (end - begin);
		line_position = (begin + end)/2; //gives the line position.
	}

}

static THD_WORKING_AREA(waCaptureImage, 512);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 239 + 240 (minimum 2 lines because reasons)
    po8030_advanced_config(FORMAT_RGB565, 0, 479, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}


static THD_WORKING_AREA(waProcessImage, 4096);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image_red[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_green[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image[IMAGE_BUFFER_SIZE] = {0};


	bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();

		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
			//extracts first 5bits of the first byte
			//takes nothing from the second byte
			image_red[i/2] = (uint8_t)img_buff_ptr[i]&RED_MASK;

			//extracts last 3bits of the first byte
			//takes first 3 bits from the second byte
			image_green[i/2] = ((uint8_t)img_buff_ptr[i]&GREEN_MASK_HIGH)<<5 | ((uint8_t)img_buff_ptr[i+1]&GREEN_MASK_LOW)>>3;
		}

		for (uint16_t i = 0; i < IMAGE_BUFFER_SIZE; i++){
			if (image_red[i]>image_green[i]){
				image [i] = image_red[i];
			}
			else{
				image [i] = image_green[i];
			}
		}

		extract_line(image);

		for(uint16_t i = (2*(IMAGE_BUFFER_SIZE/2 + LEFT_SHIFT)) ; i < (2 * (IMAGE_BUFFER_SIZE/2 + RIGHT_SHIFT)) ; i+=2){
			
			image_red_ave 	= image_red_ave     + image_red[i/2];
			image_green_ave = image_green_ave 	+ image_green[i/2];
		}

		image_red_ave = image_red_ave/(RIGHT_SHIFT - LEFT_SHIFT);
		image_green_ave = image_green_ave/(RIGHT_SHIFT - LEFT_SHIFT);


		if(send_to_computer){
			//sends to the computer the image
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}
		//invert the bool
		send_to_computer = !send_to_computer;
    }
}


uint16_t get_line_position(void){
	return line_position;
}


uint16_t get_image_red_moy(void) {
	return image_red_ave;
}

uint16_t get_image_green_moy(void) {
	return image_green_ave;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
