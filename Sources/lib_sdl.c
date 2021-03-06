#include <err.h>
#include <SDL2/SDL_image.h>
#include "../Header/lib_sdl.h"

SDL_Window *create_window(size_t width, size_t height)
{
	SDL_Window		*window;

	window = SDL_CreateWindow(
			"AEyes Viewer",
			0,
			0,
			width,
			height,
			SDL_WINDOW_SHOWN
			);

	return window;
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
	SDL_Renderer	*renderer;

	renderer = SDL_CreateRenderer(
			window,
			-1,
			0
			);

	return renderer;
}

SDL_Texture *create_texture(SDL_Renderer *renderer, size_t width, size_t height)
{
	SDL_Texture		*texture;
	texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);
	return texture;
}

SDL_Surface *create_surface(size_t width, size_t height)
{
	SDL_Surface		*surface;
	surface = SDL_CreateRGBSurfaceWithFormat(
			0,
			width,
			height,
			32,
			SDL_PIXELFORMAT_RGBA32
			);
	return surface;
}

SDL_Surface *load(char *filename)
{
	SDL_Surface *surface;
	SDL_Surface *tmp;

	tmp = IMG_Load(filename);

	if(tmp == NULL)
		return NULL;

	surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA32, 0);

	SDL_FreeSurface(tmp);

	return surface;
}

SDL_Texture *surface_to_texture(SDL_Surface *surface, SDL_Renderer *renderer)
{
	SDL_Texture		*texture, *tmp;

	tmp = SDL_CreateTextureFromSurface(renderer, surface);
	if(tmp == NULL)
		return NULL;

	texture = create_texture(renderer, surface->w, surface->h);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, tmp, NULL, NULL);
	SDL_DestroyTexture(tmp);
	SDL_SetRenderTarget(renderer, NULL);
	return texture;
}

SDL_Surface *texture_to_surface(SDL_Texture *texture)
{
	Uint32			format_pixels;
	SDL_Surface		*surface;
	void			*pixels;
	int				pitch, w, h;
	if(SDL_QueryTexture(texture, &format_pixels, NULL, &w, &h) != 0)
		return NULL;

	if(SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0)
		return NULL;
	
	surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, w *sizeof(Uint32), format_pixels);

	SDL_UnlockTexture(texture);

	return surface;
}

void print_texture_to_window(SDL_Texture *texture, SDL_Renderer *renderer, size_t posx, size_t posy)
{
	SDL_Rect		dst = {posx, posy, 0, 0};

	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_RenderPresent(renderer);
}

Uint32 get_pixel(SDL_Surface *surface, size_t posx, size_t posy)
{
	Uint32			*pixels;
	size_t			w, h;

	w = surface->w;
	h = surface->h;
	pixels = surface->pixels;

	if(w <= posx || h <= posy)
		return -1;

	return pixels[posy * w + posx];
}

void set_pixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, size_t posx, size_t posy)
{
	Uint32			*pixels;
	Uint32			color;
	size_t			w, h;

	w = surface->w;
	h = surface->h;
	pixels = surface->pixels;
	color = SDL_MapRGBA(surface->format, r, g, b, a);
	
	if(w <= posx || h <= posy)
		return;

	pixels[posy * w + posx] = color;
}

void init(SDL_Window **window, SDL_Renderer **renderer, size_t w, size_t h)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		errx(3, "Couldn't load SDL: %s", SDL_GetError());

	if(IMG_Init(0))
	{
		SDL_Quit();
		errx(3, "Coundn't load SDL_image : %s", IMG_GetError());
	}

	*window = create_window(w, h);
	if(*window == NULL)
	{
		IMG_Quit();
		SDL_Quit();
		errx(3, "Couldn't create window %s:", SDL_GetError());
	}
	
	*renderer = create_renderer(*window);
	if(*renderer == NULL)
	{
		SDL_DestroyWindow(*window);
		IMG_Quit();
		SDL_Quit();
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
	IMG_Quit();
	SDL_Quit();
}
