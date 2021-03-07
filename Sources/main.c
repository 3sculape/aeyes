#include <SDL2/SDL.h>
#include <err.h>
#include <gtk/gtk.h>
#include "../Header/lib_sdl.h"
#include "../Header/stack.h"
#include "../Header/color_histogram.h"


int main(int argc, char *argv[])
{
	if(argc != 2)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	        *renderer;
	SDL_Texture		*texture;
	SDL_Surface		*surface;

	init(&window, &renderer, 1450, 1050);

	surface = load_BMP(argv[1]);
	if(surface == NULL)
		errx(3, "Coudn't load %s: %s", argv[1], SDL_GetError());


	texture = surface_to_texture(surface, renderer);
	print_texture_to_window(texture, renderer, 0, 0);
	SDL_Delay(1000);
        size_t r[surface->w];
        size_t g[surface->w];
        size_t b[surface->w];
        color_histogram(surface, r, g, b);

        SDL_FreeSurface(surface);
	quit(window, renderer, texture);
	return 0;
}
