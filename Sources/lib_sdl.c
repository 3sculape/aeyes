#include <err.h>
#include "../Header/lib_sdl.h"

SDL_Window *create_window(int width, int height)
{
	SDL_Window *window;

	window = SDL_CreateWindow(
			"",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			0
			);

	return window;
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
	SDL_Renderer *renderer;

	renderer = SDL_CreateRenderer(
			window,
			-1,
			0
			);

	return renderer;
}

SDL_Texture *create_texture(SDL_Renderer *renderer, int width, int height)
{
	SDL_Texture *texture;
	texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);
	return texture;
}

SDL_Surface *load_BMP(Uint32 format, char *filename)
{
	SDL_Surface *surface, *tmp;

	tmp = SDL_LoadBMP(filename);
	if(tmp == NULL)
		return NULL;

	surface = SDL_ConvertSurfaceFormat(tmp, format, 0);

	SDL_FreeSurface(tmp);

	return surface;
}

void init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		errx(3, "Couldn't load SDL: %s", SDL_GetError());

	*window = create_window(w, h);
	if(*window == NULL)
	{
		SDL_Quit();
		errx(3, "Couldn't create window %s:", SDL_GetError());
	}
	
	*renderer = create_renderer(*window);
	if(*renderer == NULL)
	{
		SDL_Quit();
		SDL_DestroyWindow(*window);
		errx(3, "Couldn't create renderer %s:", SDL_GetError());
	}
}

void quit(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture)
{
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if(window != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
}
