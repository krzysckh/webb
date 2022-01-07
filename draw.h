#include "fontren.h"
#include "gfx.h"
#include "download.h"

typedef struct {
	int X;
	int Y;
	char *href;
	char *content;
} Link;

void render_site(char *site, int size, Font fnt, int max_X, int max_Y, int base_fontsize);

