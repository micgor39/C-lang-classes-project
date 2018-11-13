CC		= gcc
CFLAGS	= `pkg-config --cflags gtk+-3.0` -std=c99
LDLIBS	= `pkg-config --libs gtk+-3.0` -lm
FILES	= $(wildcard sources/*.c)

game: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o game $(LDLIBS)

clean: game
	rm game
