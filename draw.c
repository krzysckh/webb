#include "draw.h"

int hover_over_link(Link *links, int link_no) {
	if ((gfx_xpos() > links[link_no].X) && (gfx_xpos() < links[link_no].X_end)) {
		if ((gfx_ypos() > links[link_no].Y) && (gfx_ypos() < links[link_no].Y_end)) {
			return 1;
		}
	}
	return 0;
}

void render_site(char *site, int size, Font fnt, int max_X, int max_Y, int base_fontsize) {
	gfx_clear_color(BG_COLOR_r, BG_COLOR_g, BG_COLOR_b);
	gfx_color(FG_COLOR_r, FG_COLOR_g, FG_COLOR_b);
	gfx_clear();

	int X = 0, Y = 0, i = 0, j, k;
	int bold = 0, italic = 0, crossed = 0, header = 0;
	int linkAlloc = 0, current_link = 0, link_buff_alloc_count;
	char *img_link_buff;

	while (i < size) {
		if (site[i] == '&')
			linkAlloc ++;
		i ++;
	}

	Link *links = malloc(sizeof(Link) * linkAlloc);

	i = 0;
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
			case '\\':
				i ++;
				switch(site[i]) {
					case ' ':
						X += fnt.size * base_fontsize + 1;
						break;
					case '\n':
						break;
					default:
						render_letter(X, Y, base_fontsize, fnt, site[i]);
						X += fnt.size * base_fontsize + 1;
						break;
				}
				break;
			case '{':
				j = i;
				while (site[j] != '}')
					j ++;

				j -= i;

				img_link_buff = malloc(sizeof(char) * j);

				for (k = 0; k < j; k++) {
					img_link_buff[k] = '\0';
				}

				j = 0;
				i ++; /* hop to char after '{' */
				while (site[i] != '}') {
					img_link_buff[j] = site[i];
					i ++;
					j ++;
				}

				FILE *img_f = download_to_tmpf(img_link_buff);
				rewind(img_f);
				Image img_buff = load_ppm_image(img_f);
				render_image(img_buff, X, Y);

				Y += img_buff.height;
				X = 0;

				fclose(img_f);
				free(img_buff.pixel);
				free(img_link_buff);

				gfx_color(FG_COLOR_r, FG_COLOR_g, FG_COLOR_b);
				break;
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
			case '\t':
				X += 2 * (fnt.size * base_fontsize + 1);
				break;
			case '&':
				for (j = i; site[j] != '['; j++) {
					/* counting how much to alloc
					 * for *link_href_buff
					 */
				}

				links[current_link].href = malloc(sizeof(char) * j - i);

				link_buff_alloc_count = j - i;

				for (k = 0; k < (j - i); k++) {
					links[current_link].href[k] = '\0';
				}

				for (; site[j] != ']'; j++) {
					/* counting how much to alloc
					 * for *link_content_buff
					 * hoping that j didn't change
					 * (it shouldn't) o_O
					 */
				}

				links[current_link].content = malloc(sizeof(char) * j - i - link_buff_alloc_count);
				for (k = 0; k < (j - i - link_buff_alloc_count); k++) {
					links[current_link].content[k] = '\0';
				}

				/* --- variables alocated. time to fill them */

				i ++; /* so it won't copy '&' char */
				j = 0;
				while (site[i] != '[') {
					links[current_link].href[j] = site[i];
					i ++;
					j ++;
				}

				j = 0;
				i ++;
				while (site[i] != ']') {
					links[current_link].content[j] = site[i];
					i ++;
					j ++;
				}
				i ++; /* so ']' is not interpreted */

				links[current_link].X = X;
				links[current_link].Y = Y;

				links[current_link].X_end = X + (strlen(links[current_link].content) * fnt.size * base_fontsize);
				links[current_link].Y_end = Y + 1 * (fnt.size * base_fontsize);

				X += strlen(links[current_link].content) * (fnt.size * base_fontsize + 1);
				current_link ++;
				break;
			default:
				render_letter(X, Y, base_fontsize, fnt, site[i]);
				X += fnt.size * base_fontsize + 1;
				break;
		}
		i ++;
	}

	gfx_color(LINK_FG_COLOR_r, LINK_FG_COLOR_g, LINK_FG_COLOR_b);

	for (i = 0; i < current_link; i++) {
		/*gfx_line(links[i].X, links[i].Y, links[i].X_end, links[i].Y);*/
		/*gfx_line(links[i].X, links[i].Y, links[i].X, links[i].Y_end);*/

		/*gfx_line(links[i].X, links[i].Y_end, links[i].X_end, links[i].Y_end);*/
		/*gfx_line(links[i].X_end, links[i].Y, links[i].X_end, links[i].Y_end);*/

		/*for (j = 0; j < strlen(links[i].content); j++) {*/
			/*render_letter(links[i].X + (j * fnt.size * base_fontsize), links[i].Y, base_fontsize, fnt, links[i].content[j]);*/
		/*}*/
		render_text(
				links[i].content,
				strlen(links[i].content),
				links[i].X,
				links[i].Y,
				fnt,
				base_fontsize,
				links[i].X_end,
				links[i].Y_end,
				0,
				0,
				0
			);
	}

	while (1) {
		if ((j = gfx_wait()) == 1) {
			for (i = 0; i < current_link; i++) {
				if (hover_over_link(links, i)) {
					download(links[i].href, ".tmpf");

					for (j = 0; j < current_link; j++) {
						free(links[j].content);
						free(links[j].href);
					}
					free(links);

					return;
				}
			}
		}

		switch(j) {
			case 'r':
				return;
				break;
			case 'q':
				exit(0);
				break;
		}
	}


	for (i = 0; i < current_link; i++) {
		free(links[i].content);
		free(links[i].href);
	}
	free(links);
}


