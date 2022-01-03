CFLAGS=-Wall -Wextra -std=c89 -I.
LDFLAGS=-lX11 -lm -lcurl

all: fontren.o gfx.o download.o webb.o
	$(CC) $(CFLAGS) fontren.o gfx.o download.o webb.o $(LDFLAGS) -o webb
#webb.o:
	#$(CC) -c -o webb.o webb.c $(CFLAGS) $(LDFLAGS) -I./fontren.o
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
clean:
	rm *.o
	rm webb
