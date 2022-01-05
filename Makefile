CFLAGS=-Wall -Wextra -std=c89 -I.
LDFLAGS=-lX11 -lm -lcurl

all: fontren.o gfx.o download.o webb.o draw.o
	$(CC) $(CFLAGS) gfx.o fontren.o download.o draw.o webb.o $(LDFLAGS) -o webb
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
clean:
	rm *.o
	rm webb
