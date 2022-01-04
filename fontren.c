#include "fontren.h"
#include "gfx.h"

Font load_font(FILE *f_file) {
	Font ret;

	fseek(f_file, 0L, SEEK_END);
	size_t f_file_sz = ftell(f_file);
	fseek(f_file, 0L, SEEK_SET);

	char *f = malloc(sizeof(char) * (int)f_file_sz), BLANK_C = 0, PIXEL_C = 0;
	int c, i = 0, longest_line = 0, cur_line_len= 0, j, k, CURR_LETTER = 0;

	while ((c = fgetc(f_file)) != EOF) {
		f[i] = c;

		if (c == '\n') {
			if (cur_line_len > longest_line)
				longest_line = cur_line_len;
			cur_line_len = 0;
		}

		cur_line_len ++;
		i ++;
	}

	/* full font file now in memory as *f */

	char *token = malloc(sizeof(char) * longest_line);
	for (i = 0; i < longest_line; i++) {
		token[i] = '\0';
	}

	for (i = 0; f[i] != '\n'; i++) {
		token[i] = f[i];
	}

	if (strcmp(token, "FORMAT FRFONT") != 0) {
		fprintf(stderr, "err: font not in FRFONT format\n");
		exit(1);
	}



	ret.defines = NULL;
	ret.size = -1;
	i = (int) strlen("FORMAT FRFONT") + 1;

	while (i < f_file_sz) {
		for (j = 0; j < longest_line; j++) {
			token[j] = '\0';
		}

		j = 0;
		while (f[j + i] != ' ' && f[j + i] != '\n') {
			token[j] = f[j + i];
			j ++;
		}

		if (strcmp(token, "DEFINES") == 0) {
			for (k = 0; k < longest_line; k++)
				token[k] = 0;
			k = 0;
			if (f[j + i] == ' ')
				j ++;
			while (f[j + i] != '\n') {
				token[k] = f[j + i];
				k ++;
				j ++;
			}

			ret.defines = malloc(sizeof(char) * strlen(token));
			strcpy(ret.defines, token);
			ret.letter = malloc(sizeof(Letter) * strlen(ret.defines));
		} else if (strcmp(token, "SIZE") == 0) {
			for (k = 0; k < longest_line; k++)
				token[k] = 0;
			k = 0;
			while (f[j + i] != '\n') {
				token[k] = f[j + i];
				k ++;
				j ++;
			}

			if ((ret.size = atoi(token)) < 8) {
				fprintf(stderr, "err: error in font. got token SIZE with SIZE smaller than 8\n");
				free(token);
				free(f);
				exit(1);
			}
		} else if (strcmp(token, "BLANK") == 0) {
			for (k = 0; k < longest_line; k++)
				token[k] = 0;
			k = 0;
			while (f[j + i] != '\n') {
				token[k] = f[j + i];
				k ++;
				j ++;
			}

			BLANK_C = token[1];
		} else if (strcmp(token, "PIXEL") == 0) {
			for (k = 0; k < longest_line; k++)
				token[k] = 0;
			k = 0;
			while (f[j + i] != '\n') {
				token[k] = f[j + i];
				k ++;
				j ++;
			}

			PIXEL_C = token[1];
		} else if (strcmp(token, "DEF") == 0) {
			if (ret.defines == NULL) {
				fprintf(stderr, "err: error in font. got token DEF, but DEFINES is not specified\n");
				free(token);
				free(f);
				exit(1);
			}

			if (ret.size == -1) {
				fprintf(stderr, "err: error in font. got token DEF, but SIZE is not specified\n");
				free(token);
				free(f);
				exit(1);
			}

			if (PIXEL_C == 0 || BLANK_C == 0) {
				printf("err: error in font. got token DEF, but BLANK or PIXEL is not defined\n");
				free(token);
				free(f);
				exit(1);
			}

			printf("Font: Defined letter %c\n", ret.defines[CURR_LETTER]);
			while (f[j + i] != '\n')
				j ++;
			/* go to next line */

			ret.letter[CURR_LETTER].pixel = malloc(sizeof(int) * (ret.size * ret.size));


			k = 0;
			while (k < ret.size * ret.size) {
				if (f[j + i] == BLANK_C) {
					ret.letter[CURR_LETTER].pixel[k] = 0;
					k ++;
				} else if (f[j + i] == PIXEL_C) {
					ret.letter[CURR_LETTER].pixel[k] = 1;
					k ++;
				} else if (f[j + i] == '\n') {
					/* nothing */
				} else {
					fprintf(stderr, "err: fatal error in font. char %c (%d) not supposed to be in a definition of a letter\nexiting. may cause memory leaks.", f[j+i], CURR_LETTER);
					exit(1);
				}

				j ++;
			}
			CURR_LETTER ++;
		} else {
			printf("err: error in font. unexpected token \"%s\"\n", token);
			free(token);
			free(f);
			exit(1);
		}
		
		j ++;
		i += j;
	}

	free(token);
	free(f);
	fclose(f_file);
	return ret;
}

void draw_pixel(int X, int Y, int Size) {
	int i;
	for (i = X; i < X + Size; i++) {
		gfx_line(i, Y, i, Y + Size);
	}
}


void render_letter(int X, int Y, int Size, Font fnt, char lttr) {
	int i, xn = X, yn = Y, addr = 0;
	for (i = 0; i < strlen(fnt.defines); i++) {
		if (fnt.defines[i] != lttr) {
			addr ++;
		} else {
			break;
		}
	}

	if (fnt.defines[addr] != lttr) {
		printf("render_letter(): font does not support '%c', defaulting to fnt.defines[0] (= %c)\n", lttr, fnt.defines[0]);
		addr = 0;
	}

	for (i = 0; i < (fnt.size * fnt.size); i++) {
		if (fnt.letter[addr].pixel[i])
			draw_pixel(xn, yn, Size);
		xn += Size;
		if ((i % fnt.size == 0) && i != 0) {
			yn += Size;
			xn = X;
		}
	}
	
}

void render_text(char *text, int length, int START_X, int START_Y, Font fnt, int Size, int max_width, int max_height, int word_spacing, int line_spacing, int letter_spacing) {
	int i, x = START_X, y = START_Y;

	for (i = 0; i < length; i++) {
		if (x >= max_width) {
			x = START_X;
			y += fnt.size * Size + line_spacing;
		}

		if (y >= max_height) {
			return;
		}
		switch (text[i]) {
			case '\n':
				y += fnt.size * Size + line_spacing;
				x = START_X;
				break;
			case ' ':
				x += fnt.size * Size + word_spacing;
				break;
			case '\t':
				x += fnt.size + word_spacing;
				break;
			default:
				render_letter(x, y, Size, fnt, text[i]);
				x += fnt.size * Size + letter_spacing;
				break;
		}
	}
}

/*
int main (void) {
	gfx_open(300, 300, "fontren demo");

	gfx_color(255, 255, 255);

	Font fnt = load_font(fopen("standard.frfont", "rb+"));

	while (1) {
		render_letter(0, 0, 2, fnt, 'H');
		render_letter(20, 0, 2, fnt, 'e');
		render_letter(40, 0, 2, fnt, 'l');
		render_letter(60, 0, 2, fnt, 'l');
		render_letter(80, 0, 2, fnt, 'o');
		gfx_flush();
	}

	int i;

	for (i = 0; i < strlen(fnt.defines) - 1; i++) {
		free(fnt.letter[i].pixel);
	}
	free(fnt.defines);
	free(fnt.letter);
	return 0;
}
*/
