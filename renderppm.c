#include "renderppm.h"
#include <stdio.h>

/* renderppm.c only supports up to 256b PPM files */

int _imagemagick_convert_img_to_ppm(char *fname) {
	int ret;
	if (fork() == 0) {
		ret = execl("/usr/bin/convert", "convert", fname, "/tmp/_im_webb_ppm.ppm", (char*) NULL);
		printf("really tryuing ret = %d\n", ret);
	}
	return ret;
}

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

void render_image_smaller(Image img, int X, int Y, int ommitPix) {
	printf("rendering smaller ommit = %d\n", ommitPix);
	int startX = X;
	int i;
	for (i = 0; i < (img.width * img.height); i+=ommitPix) {
		if ((i + 1) % img.width/ommitPix == 0) {
			Y ++;
			X = startX;
			i += (ommitPix-1) * (img.width);
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
		printf("load_ppm_image(): err: not a ppm image (%s != P6) Trying converting with ImageMagick\n", header);
		FILE *_im_tmp = fopen("/tmp/_im_webb_tmp", "w");
		rewind(img_f);
		while ((i = fgetc(img_f)) != EOF) {
			fputc(i, _im_tmp);
		}

		fclose(_im_tmp);
		if (!_imagemagick_convert_img_to_ppm("/tmp/_im_webb_tmp")) {
			return load_ppm_image(fopen("res/not_ppm.ppm", "rb+"));
		} else {
			return load_ppm_image(fopen("/tmp/_im_webb_ppm.ppm", "rb+"));
		}
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
						return load_ppm_image(fopen("res/not_ppm.ppm", "rb+"));
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
		return load_ppm_image(fopen("res/not_ppm.ppm", "rb+"));
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


