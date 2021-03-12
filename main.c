#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "utils/lib_sdl.h"
#include "utils/stack.h"
#include "algos/basic.h"
#include "algos/convert.h"
#include "algos/color.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Surface		*surface;

	init(&window, &renderer, 1366, 768);

	surface = load(argv[1]);
	if(surface == NULL)
	{
		quit(window, renderer, NULL);
		errx(3, "Coudn't load %s", argv[1]);
	}

	white_balance(surface, 100);
	texture = surface_to_texture(surface, renderer);

	print_texture_to_window(texture, renderer);
    saveJPG(argv[2], surface);
	SDL_Delay(3000);

	SDL_FreeSurface(surface);
	quit(window, renderer, texture);

	return 0;
}
