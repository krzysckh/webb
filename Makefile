CFLAGS=-Wall -Wextra -std=c89 -I. -ggdb -Ofast
LDFLAGS=-lX11 -lm -lcurl

all: fontren.o gfx.o download.o webb.o draw.o renderppm.o ui.o
	$(CC) $(CFLAGS) gfx.o fontren.o download.o draw.o webb.o renderppm.o ui.o $(LDFLAGS) -o webb
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 
clean:
	rm -f webb *.o
test: all
	./webb -X 1920 -Y 1200 -s 2
