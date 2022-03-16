#include "ui.h"
/*#include "fontren.h"*/ /*included in ui.h */
#include "gfx.h"
#include "download.h"
#include "renderppm.h"
#include "config.h"

typedef struct {
	int X;
	int Y;
	int X_end;
	int Y_end;
	char *href;
	char *content;
} Link;

void render_site(char *site, int size, Font fnt, int max_X, int max_Y, int base_fontsize);
