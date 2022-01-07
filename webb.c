#include "webb.h"
#include "gfx.h"

#include "config.h"

int main (int argc, char *argv[]) {
	int xs = 1000, ys = 1000;

	int opt;
	char *font_name = "fonts/standard.frfont";
	char *web_download = NULL;
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

	FILE *fnt_f = fopen(font_name, "rb+");
	if (fnt_f == NULL) {
		printf("cannot open font %s\n", font_name);
		exit(1);
	}

	Font fnt = load_font(fnt_f);

	int i = 0, c;

	if (web_download == NULL) {
		FILE *welcome = fopen("sites/welcome.uu", "r");
		FILE *tmpf = fopen(".tmpf", "w");
	
		while ((c = fgetc(welcome)) != EOF) {
			fputc(c, tmpf);
		}
		fclose(welcome);
		fclose(tmpf);
	} else {
		download(web_download, ".tmpf");
	}

	FILE *fp = fopen(".tmpf", "rb+");
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	char *in = malloc(sizeof(char) * sz);
	while ((c = fgetc(fp)) != EOF) {
		in[i] = c;
		i ++;
	}

	int run = 1;
	while (run) {
		gfx_clear_color(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2]);
		gfx_color(FG_COLOR[0], FG_COLOR[1], FG_COLOR[2]);
		gfx_clear();
		/*gfx_flush();*/

		fclose(fp);
		render_site(in, sz, fnt, xs, ys, fsize);
		in = NULL;
		free(in);

		fp = fopen(".tmpf", "rb+");
		fseek(fp, 0L, SEEK_END);
		sz = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		in = malloc(sizeof(char) * sz);
		i = 0;
		while ((c = fgetc(fp)) != EOF) {
			in[i] = c;
			i ++;
		}
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
