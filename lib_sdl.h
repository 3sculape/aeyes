#ifndef LIB_SDL_H
#define LIB_SDL_H

#include <SDL2/SDL.h>

//Create a SDL_Window object with size width and height.
//Return NULL if an error occured.
SDL_Window *create_window(int width, int height);

//Create a SDL_Renderer object for the SDL_Window object window.
//Return NULL when an error occured.
SDL_Renderer *create_renderer(SDL_Window *window);

//Create a texture with the renderer format and size width and height.
//Return NULL when an error occured.
SDL_Texture *create_texture(SDL_Renderer *renderer, int width, int height);

//Load an Bitmap image from the path filename with the format format.
//Return NULL if an error occured.
SDL_Surface *load_BMP(Uint32 format, char *filename);

//Initialize SDL and create :
//a new window with size w and h to the pointer **window
//a new renderer liked to **window to the pointer **renderer
//Clear everything if it fail.
void init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);

//Destroy all parameters, put a NULL as parameters if nothing to destroy.
//Quit SDL.
void quit(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

#endif
