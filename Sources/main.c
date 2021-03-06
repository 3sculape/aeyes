#include <SDL2/SDL.h>
#include <err.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "../Header/lib_sdl.h"
#include "../Header/stack.h"
#include "../algorithms/basic.h"
#include "../algorithms/convert.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Surface		*surface;

	double hsv[3];
	double rgb[3];
	rgb_to_hsv(222, 0, 32, hsv);
	hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

	printf("R (222): %f\nG (0):   %f\nB (32):  %f\n", rgb[0], rgb[1], rgb[2]);

	init(&window, &renderer, 1366, 768);

	surface = load_BMP(argv[1]);
	if(surface == NULL)
		errx(3, "Coudn't load %s: %s", argv[1], SDL_GetError());

	negative(surface);

	texture = surface_to_texture(surface, renderer);
	
	print_texture_to_window(texture, renderer, 0, 0);
	SDL_Delay(10000);
	SDL_FreeSurface(surface);

	quit(window, renderer, texture);

	return 0;
}
