#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "utils/lib_sdl.h"
#include "algos/basic.h"
#include "algos/convert.h"
#include "algos/color.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture2;
	SDL_Surface		*surface;

	init(&window, &renderer, 1366, 768);

    surface = load(argv[1]);
	if(surface == NULL)
	{
		quit(window, renderer, NULL);
		errx(3, "Coudn't load %s", argv[1]);
	}

	texture2 = surface_to_texture(surface, renderer);

    SDL_Texture *texture = resize(texture2, renderer, 2);

	print_texture_to_window(texture, renderer);
    SDL_FreeSurface(surface);
    surface = texture_to_surface(texture, renderer);


    savePNG(argv[2], surface);
	SDL_Delay(3000);
        free(surface->pixels);
	SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture2);
	quit(window, renderer, texture);

	return 0;
}
