#include "webb.h"

int main (void) {
	gfx_open(1000, 1000, "webb");

	Font fnt = load_font(fopen("standard.frfont", "rb+"));
	render_text("Hello, World!", 10, 10, fnt, 2, 900, 900, fnt.size, fnt.size/2);

	while (1) {
		gfx_wait();
	}
	
	return 0;
}
