#include "draw.h"

void render_site(char *site, int size, Font fnt, int max_X, int max_Y, int base_fontsize) {
	int X = 0, Y = 0, i = 0;
	int bold = 0, italic = 0, crossed = 0, header = 0;
	while (i < size) {
		if (X > max_X) {
			printf("no more X space\n");
			Y += fnt.size * base_fontsize + 2;
			X = 0;
		}
		if (Y > max_Y) {
			printf("no more Y space\n");
			return;
		}

		switch(site[i]) {
			case '*':
				if (bold) {
					bold = 0;
				} else {
					bold = 1;
				}
				break;
			case '-':
				if (crossed) {
					crossed = 0;
				} else {
					crossed = 1;
				}
				break;
			case '_':
				if (italic) {
					italic = 0;
				} else {
					italic = 1;
				}
				break;
			case '+':
				if (header) {
					header = 0;
					Y += fnt.size + 2 + 1;
					base_fontsize -= 1;
				} else {
					header = 1;
					base_fontsize += 1;
				}
				break;
			case '\n':
				Y += fnt.size * base_fontsize + 1;
				if (bold) bold = 0;
				if (italic) italic = 0;
				if (crossed) crossed = 0;
				if (header) {
					header = 0;
					base_fontsize -= 1;
				}
				X = 0;
				break;
			case ' ':
				X += fnt.size * base_fontsize + 1;
				break;
			default:
				render_letter(X, Y, base_fontsize, fnt, site[i]);
				X += fnt.size * base_fontsize + 1;
				break;
		}
		i ++;
	}
}


