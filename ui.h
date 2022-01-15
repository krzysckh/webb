#include <stdio.h>
#include <stdlib.h>

#include "fontren.h"

typedef struct {
	int X1;
	int Y1;
	int X2;
	int Y2;
} click_box;

int click_menu(int X, int Y, char *options[], int optn, Font fnt, int padding, int fsize);
int is_in_clbox(click_box bx);
char *searchbar(int X, int Y, int width, Font fnt, int padding, char *prompt, int MAX_LEN);
