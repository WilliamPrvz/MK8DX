#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#define THRESHOLD	10
#define RED_MASK	0xF8
#define GREEN_MASK_HIGH	0x07
#define GREEN_MASK_LOW	0xE0
#define RED_MASK	0x1F

void process_image_start(void);
uint16_t get_line_position(void);
uint16_t get_image_red_moy(void);
uint16_t get_image_green_moy(void);
uint16_t get_image_blue_moy(void);


#endif /* PROCESS_IMAGE_H */
