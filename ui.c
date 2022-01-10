#include "ui.h"

int is_in_clbox(click_box bx) {
	int i;
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

int click_menu(int X, int Y, char *options[], int optn, Font fnt, int padding, int fsize) {
	int ret = -1, xn = X, yn = Y, i;
	int maxw = 0;

	click_box *optboxes = malloc(sizeof(click_box) * optn);

	for (i = 0; i < optn; i++) {
		maxw = (strlen(options[i]) > maxw) ? strlen(options[i]) : maxw;
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
