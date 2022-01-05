#include "webb.h"

int main (int argc, char *argv[]) {
	int xs = 1000, ys = 1000;

	int opt;
	char *font_name = "fonts/standard.frfont";
	char *web_download = "http://krzysckh.org/index.html";
	int fsize = 1;
	int lspacing = 0;
	while ((opt = getopt(argc, argv, "hf:X:Y:w:s:l:")) != -1) {
		switch (opt) {
			case 'h':
				printf(
						"webb - web browser (in the future)\n"
						"webb [-h] [-f font.frfont] [-X xsize] [-Y ysize] [-s fontsize] [-l letter_spacing] [-w web adress]\n"
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
			case 's':
				fsize = atoi(optarg);
				break;
			case 'l':
				lspacing = atoi(optarg);
				break;
			case 'w':
				web_download = optarg;
				break;
			default:
				exit(1);
				break;
		}
	}

	gfx_open(xs, ys, "webb");
	gfx_clear_color(34, 34, 34);
	gfx_color(222,222,222);
	gfx_flush();

	FILE *fnt_f = fopen(font_name, "rb+");
	if (fnt_f == NULL) {
		printf("cannot open font %s\n", font_name);
		exit(1);
	}

	Font fnt = load_font(fnt_f);


	download(web_download, ".tmpf");

	FILE *fp = fopen(".tmpf", "rb+");
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	int i = 0, c;

	char *in = malloc(sizeof(char) * sz);
	while ((c = fgetc(fp)) != EOF) {
		in[i] = c;
		i ++;
	}

	int run = 1;
	while (run) {
		render_site(in, sz, fnt, xs, ys, fsize);

		switch (c = gfx_wait()) {
			case 'q':
				run = 0;
				break;
			case ']':
				fsize ++;
				gfx_clear();
				break;
			case '[':
				fsize --;
				gfx_clear();
				break;
			default:
				printf("%c (%d)\n", c, c);
				break;
		}
		gfx_flush();
	}

	/*
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

	int run = 1;
	while (run) {
		render_text(inf, (int) sz-1, 10, 10, fnt, fsize, xs, ys, 5, fnt.size/2, lspacing);
		inputbar(0, 0, 100, 100, "gaming");

		switch (c = gfx_wait()) {
			case 'q':
				run = 0;
				break;
			case ']':
				fsize ++;
				gfx_clear();
				break;
			case '[':
				fsize --;
				gfx_clear();
				break;
			default:
				printf("%c (%d)\n", c, c);
				break;
		}
		gfx_flush();
	}

	fclose(fp);
	free(inf);
	*/

	free(fp);
	free(in);
	fclose(fnt_f);
	return 0;
}
