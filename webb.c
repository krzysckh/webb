#include "webb.h"

int main (int argc, char *argv[]) {
	gfx_open(1000, 1000, "webb");
	gfx_clear_color(34, 34, 34);
	gfx_color(222,222,222);
	gfx_flush();

	int xs = 1000, ys = 1000;

	int opt;
	char *font_name = "fonts/standard.frfont";
	char *web_download = "http://krzysckh.org/index.html";
	while ((opt = getopt(argc, argv, "hf:X:Y:w:")) != -1) {
		switch (opt) {
			case 'h':
				printf(
						"webb - web browser (in the future)\n"
						"webb [-h] [-f font.frfont] [-X xsize] [-Y ysize] [-w web adress]\n"
					);
				exit(0);
				break;
			case 'f':
				font_name = optarg;
				break;
			case 'X':
				xs = atoi(optarg);
				break;
			case 'Y':
				ys = atoi(optarg);
				break;
			case 'w':
				web_download = optarg;
				break;
			default:
				exit(1);
				break;
		}
	}
	FILE *fnt_f = fopen(font_name, "rb+");
	if (fnt_f == NULL) {
		printf("cannot open font %s\n", font_name);
		exit(1);
	}

	Font fnt = load_font(fnt_f);

	download(web_download, ".tmpf");
	
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

	while (1) {
		render_text(inf, (int) sz-1, 10, 10, fnt, 1, xs, ys, 5, fnt.size/2, 1);
		if (gfx_wait() == 'q') {
			break;
		}
	}

	fclose(fp);
	free(inf);
	fclose(fnt_f);
	return 0;
}
