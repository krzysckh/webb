CFLAGS=-Wall -Wextra -std=c89 -I.
LDFLAGS=-lX11 -lm -lcurl

all: fontren.o gfx.o download.o webb.o
	$(CC) $(CFLAGS) fontren.o download.o webb.o gfx.o $(LDFLAGS) -o webb
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
clean:
	rm *.o
	rm webb
