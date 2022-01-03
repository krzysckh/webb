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

void render_text(char *, int, int, Font, int, int, int, int, int);
