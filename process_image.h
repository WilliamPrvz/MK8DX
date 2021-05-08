#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

//float get_distance_cm(void);


void process_image_start(void);
uint16_t get_line_position(void);
uint16_t get_image_red_moy(void);
uint16_t get_image_green_moy(void);
uint16_t get_image_blue_moy(void);


#define THRESHOLD 10

#endif /* PROCESS_IMAGE_H */
