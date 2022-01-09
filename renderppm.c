#include "renderppm.h"

/* renderppm.c only supports up to 256b PPM files */

void render_image(Image img, int X, int Y) {
	int startX = X;
	int i;
	for (i = 0; i < (img.width * img.height); i++) {
		if ((i + 1) % img.width == 0) {
			Y ++;
			X = startX;
		}
		gfx_color(img.pixel[i].r, img.pixel[i].g, img.pixel[i].b);
		gfx_point(X, Y);
		/*printf("%d. drew (%d %d %d) at %d %d\n", i, img.pixel[i].r, img.pixel[i].g, img.pixel[i].b, X, Y);*/
		X ++;
	}
}

Image load_ppm_image (FILE *img_f) {
	Image ret;
	ret.width = 0;
	ret.height = 0;
	
	/* i could read the whole file into the memory,
	 * but i think this approach will be faster
	 */

	uint8_t b;
	int i, Maxval = 0;

	char header[3];
	header[0] = fgetc(img_f);
	header[1] = fgetc(img_f);
	header[2] = '\0';

	char img_sz_buff[15];

	if (strcmp(header, "P6") != 0) {
		printf("load_ppm_image(): err: not a ppm image (%s != P6)\n", header);
		return ret;
		/* TODO: create an image that will show when an image is invalid and return it */
	}

	while (Maxval == 0) {
		b = fgetc(img_f);
		switch (b) {
			case '#':
				while ((b = fgetc(img_f)) != '\n');
				break;
			default:
				if (isspace(b))
					break;
				for (i = 0; i < 15; i++) {
					img_sz_buff[i] = '\0';
				}

				i = 0;
				while (1) {
					if (isspace(b)) {
						break;
					}
					if (i > 15) {
						printf("load_ppm_image(): err: header too long for buffer (%s)\n", img_sz_buff);
						return ret;
					}

					img_sz_buff[i] = b;
					i++;
					b = fgetc(img_f);
				}
				
				if (ret.width == 0) {
					ret.width = atoi(img_sz_buff);
				} else if (ret.height == 0) {
					ret.height = atoi(img_sz_buff);
				} else {
					Maxval = atoi(img_sz_buff);
				}
				break;
		}
	}

	if (Maxval > 255) {
		printf("load_ppm_image(): err: now supporting just up to 255 Maxval in ppm images (maxval set to %d)\n", Maxval);
		return ret;
	}

	ret.pixel = malloc(sizeof(ImagePixel) * (ret.height * ret.width));

	i = 0;
	while (i < (ret.height * ret.width)) { /* bad way to do this. blindly trusting that the image is good */
		ret.pixel[i].r = fgetc(img_f);
		ret.pixel[i].g = fgetc(img_f);
		ret.pixel[i].b = fgetc(img_f);

		/*ret.pixel[i].r = (ret.pixel[i].r / Maxval) * (255 / Maxval);*/
		/*ret.pixel[i].g = (ret.pixel[i].g / Maxval) * (255 / Maxval);*/
		/*ret.pixel[i].b = (ret.pixel[i].b / Maxval) * (255 / Maxval);*/
		/*need to fix that*/

		i ++;
	}

	return ret;
}


