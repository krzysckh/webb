#include "webb.h"
#include "gfx.h"
#include <stdio.h>

int main (void) {
	gfx_open(1000, 1000, "webb");
	gfx_clear_color(34, 34, 34);
	gfx_color(222,222,222);
	gfx_flush();

	Font fnt = load_font(fopen("standard.frfont", "rb+"));

	download("http://krzysckh.org/index.html", ".tmpf");
	
	FILE *fp = fopen(".tmpf", "rb+");
	fseek(fp, 0L, SEEK_END);
	size_t sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	int c, i = 0;
	char *inf = malloc(sizeof(char) * (int)sz);
	while ((c = fgetc(fp)) != EOF) {
		inf[i] = c;
		i ++;
	}

	printf("sz = %d\n", sz);
	int xs, ys;

	while (1) {
		render_text(inf, (int) sz-1, 10, 10, fnt, 1, 1000, 1000, 5, fnt.size/2, 1);
		if (gfx_wait() == 'q') {
			break;
		}
	}

	fclose(fp);
	free(inf);
	return 0;
}
