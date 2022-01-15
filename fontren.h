#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"

typedef struct {
	int *pixel;
} Letter;

typedef struct {
	int size;
	char *defines;
	Letter *letter;
} Font;

Font load_font(FILE*);

void draw_pixel(int, int, int);

void render_letter(int, int, int, Font, char);

void render_text(char *text, int length, int START_X, int START_Y, Font fnt, int Size, int max_width, int max_height, int word_spacing, int line_spacing, int letter_spacing);
