CFLAGS=-Wall -Wextra -std=c89 -I
LDFLAGS=-lX11 -lm -lcurl

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
all: fontren.o gfx.o download.o webb.o
	$(CC) $(CFLAGS) fontren.o webb.o gfx.o download.o $(LDFLAGS) -o webb
clean:
	rm *.o
	rm webb
