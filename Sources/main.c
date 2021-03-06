#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include "../Header/lib_sdl.h"
#include "../Header/stack.h"


int main(int argc, char *argv[])
{
	if(argc != 2)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Surface		*surface;

	init(&window, &renderer, 1450, 1050);

	surface = load(argv[1]);
	if(surface == NULL)
	{
		quit(window, renderer, NULL);
		errx(3, "Coudn't load %s", argv[1]);
	}

	texture = surface_to_texture(surface, renderer);
	
	print_texture_to_window(texture, renderer, 0, 0);
	SDL_Delay(10000);

	quit(window, renderer, texture);

	return 0;
}
