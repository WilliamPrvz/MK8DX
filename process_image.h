#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

// masks to get the corresponding bits for the 2 color bytes
#define RED_MASK	0xF8
#define GREEN_MASK_HIGH	0x07
#define GREEN_MASK_LOW	0xE0

// shifting pixels to avoid taking detecting only the black line of the coloured stickers
#define LEFT_SHIFT		100		
#define RIGHT_SHIFT		120

void process_image_start(void);
uint16_t get_line_position(void);
uint16_t get_image_red_moy(void);
uint16_t get_image_green_moy(void);


#endif /* PROCESS_IMAGE_H */
