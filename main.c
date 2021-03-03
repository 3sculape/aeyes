#include <SDL2/SDL.h>
#include <err.h>
#include <gtk/gtk.h>
#include "lib_sdl.h"


int main(int argc, char *argv[])
{
	if(argc != 2)
		errx(3, "Usage : ./gtk <filename>");

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Texture		*tmp;
	SDL_Surface		*surface;
	SDL_Rect		dst = {0, 0, 0, 0};

	init(&window, &renderer, 1450, 1050);

	surface = load_BMP(SDL_PIXELFORMAT_RGBA8888, argv[1]);
	if(surface == NULL)
		errx(3, "Coudn't load %s: %s", argv[1], SDL_GetError());

	tmp = SDL_CreateTextureFromSurface(renderer, surface);
	if(tmp == NULL)
		errx(3, "Coudn't create texture from surface: %s", SDL_GetError());


	texture = create_texture(renderer, surface->w, surface->h);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, tmp, NULL, NULL);
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(tmp);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_RenderPresent(renderer);
	SDL_Delay(10000);

	quit(window, renderer, texture);

	return 0;
}
