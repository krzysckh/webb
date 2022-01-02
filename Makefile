CFLAGS=-Wall -Wextra -std=c89
LDFLAGS=-lX11 -lm

all: 
	$(CC) fontren.c gfx.c $(CFLAGS) -o fontren $(LDFLAGS)
clean:
	rm fontren
