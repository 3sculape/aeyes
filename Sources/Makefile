CC = gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl2` `pkg-config --cflags --libs gtk+-3.0` -MMD
CFLAGS = -Wall -Werror -Wextra -lm -std=c99 -O3 -g -rdynamic -pthread -pipe
OBJ = main.o stack.o lib_sdl.o color_histogram.o
DEP = ${OBJ:.o=.d}
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image `pkg-config --cflags --libs gtk+-3.0` -export-dynamic


main: ${OBJ}

all: main

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
