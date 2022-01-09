#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "gfx.h"

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ImagePixel;

typedef struct {
	int width;
	int height;
	ImagePixel *pixel;
} Image;

void render_image(Image img, int X, int Y);
void render_image_smaller(Image img, int X, int Y, int ommitPix);
Image load_ppm_image (FILE *img_f);
