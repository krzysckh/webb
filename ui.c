#include "ui.h"

int is_in_clbox(click_box bx) {
	if (
			gfx_xpos() < bx.X2 && \
			gfx_xpos() > bx.X1 && \
			gfx_ypos() < bx.Y2 && \
			gfx_ypos() > bx.Y1 \
	) {
		return 1;
	} else {
		return 0;
	}
}

void draw_rect(int x1, int y1, int x2, int y2) {
	int i;
	
	for (i = y1; i < y2; i++) {
		gfx_line(x1, i, x2, i);
	}
}

int click_menu(int X, int Y, char *options[], int optn, Font fnt, int padding, int fsize) {
	int ret = -1, xn = X, yn = Y, i;
	int maxw = 0;

	click_box *optboxes = malloc(sizeof(click_box) * optn);

	for (i = 0; i < optn; i++) {
		maxw = ((int)strlen(options[i]) > maxw) ? (int)strlen(options[i]) : maxw;
		/* calculates the longest opt */
	}


	for (i = 0; i < optn; i++) {
		gfx_color(200, 200, 200);
		gfx_line(xn, yn, (xn + (maxw * fnt.size * fsize) + (padding * 2)), yn);
		gfx_line(xn, yn, xn, yn + fnt.size * fsize + (padding * 2));
		gfx_line(xn + (maxw * fnt.size * fsize) + (padding * 2), yn, xn + (maxw * fnt.size * fsize) + (padding * 2), yn + fnt.size * fsize + (padding * 2));
		gfx_line(xn, yn + fnt.size * fsize+ (padding * 2), xn + (maxw * fnt.size * fsize) + (padding * 2), yn + fnt.size * fsize + (padding * 2));

		gfx_color(10, 200, 200);
		render_text(
				options[i],
				strlen(options[i]),
				xn + padding,
				yn + padding,
				fnt,
				fsize,
				xn + (maxw * fnt.size * fsize),
				yn + (fnt.size * fsize) + 10,
				0,
				0,
				0
		);

		optboxes[i].X1 = xn;
		optboxes[i].Y1 = yn;
		optboxes[i].X2 = (xn + (maxw * fnt.size * fsize) + (padding * 2));
		optboxes[i].Y2 = (yn + fnt.size * fsize + (padding * 2));

		yn += fnt.size * fsize + (padding * 2);
	}

	while (1) {
		if (gfx_wait() == 1) {
			for (i = 0; i < optn; i++) {
				if (is_in_clbox(optboxes[i])) {
					ret = i;
					break;
				}
			}
			break;
		}
	}

	free(optboxes);
	return ret;
}

char *searchbar(int X, int Y, int width, Font fnt, int padding, char *prompt, int MAX_LEN) {
	char *ret = malloc(sizeof(char) * MAX_LEN);
	int c, i;

	for (i = 0; i < MAX_LEN; i++) {
		ret[i] = '\0';
	}

	i = 0;

	gfx_color(230, 230, 230);
	draw_rect(X, Y, X + width + (padding * 2), Y + (padding * 2) + fnt.size);

	while ((c = gfx_wait()) != 13) {
		if (c == 8 && i != 0) {
			i -= 1;
			ret[i] = '\0';
			gfx_color(230, 230, 230);
			draw_rect(X, Y, X + width + (padding * 2), Y + (padding * 2) + fnt.size);
			gfx_color(22,22,22);
			render_text(ret, strlen(ret), X + padding, Y + padding, fnt, 1, width + X + (padding * 2), Y + (padding * 2) + (fnt.size * 1), 0, 0, 0);
			continue;
		}
		gfx_color(230, 230, 230);
		draw_rect(X, Y, X + width + (padding * 2), Y + (padding * 2) + fnt.size);

		ret[i] = c;

		gfx_color(22,22,22);
		render_text(ret, strlen(ret), X + padding, Y + padding, fnt, 1, width + X + (padding * 2), Y + (padding * 2) + (fnt.size * 1), 0, 0, 0);
		printf("ret = %s\n", ret);
		printf("c = %c (%d) \n", c, c);

		i ++;
	}

	return ret;
}
