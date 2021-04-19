CC=gcc -fsanitize=address -rdynamic -export-dynamic
CPPFLAGS=`pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -MMD

CFLAGS=-Wall -Wextra -std=c99 -O3 -g
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

# Source for SDL and GTK compilation
LDLIBS=`pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` `pkg-config --libs gsl` -lSDL2_image -lm -lpng -ljpeg -lSDL2_gfx -lexif

# Source for SDL and GTK compilation
GTK_SRC= main.c utils/lib_exif.c utils/lib_sdl.c utils/stack.c algos/basic.c algos/convert.c algos/color.c algos/blurs.c algos/unsharp_masking.c algos/utility.c algos/whites.c algos/secret_sauce.c algos/color_histogram.c algos/trailing.c algos/canny_edge_detector.c

GTK_OBJ=$(GTK_SRC:.c=.o)
GTK_DEP=$(GTK_SRC:.c=.d)

all: gtk

gtk: $(GTK_OBJ)
	$(CC) $(GTK_OBJ) -o gtk $(LDLIBS)

.PHONY: clean

clean:
	$(RM) $(GTK_OBJ) $(GTK_DEP) gtk
