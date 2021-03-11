CC=gcc
CPPFLAGS=`pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -MMD
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS=`pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` `pkg-config --libs gsl` -lSDL2_image -lm -lpng -ljpeg

# Source for SDL and GTK compilation
GTK_SRC= main.c utils/lib_sdl.c algos/basic.c algos/convert.c algos/color.c
GTK_OBJ=$(GTK_SRC:.c=.o)
GTK_DEP=$(GTK_SRC:.c=.d)

all: gtk

gtk: $(GTK_OBJ)
	$(CC) $(LDLIBS) $(GTK_OBJ) -o gtk

.PHONY: clean

clean:
	$(RM) $(GTK_OBJ) $(GTK_DEP) gtk
