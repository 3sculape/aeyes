#ifndef LIB_SDL_H
#define LIB_SDL_H

#include <SDL2/SDL.h>

//Create a SDL_Window object with size width and height and the SDL_Renderer object linked to it.
//Return NULL if an error occured.
void create_window(size_t width, size_t height, SDL_Window **window, SDL_Renderer **renderer);

//Create a texture with the renderer format and size width and height.
//Return NULL when an error occured.
SDL_Texture *create_texture(SDL_Renderer *renderer, size_t width, size_t height);

//Create a texture with the renderer format and size width and height.
//Return NULL when an error occured.
SDL_Surface *create_surface(size_t width, size_t height);

//Load a JPG or PNG image from the path filename.
//Return NULL if an error occured.
SDL_Surface *load(char *filename);

//Save a SDL_Surface to the path filename with a png format.
//Return NULL if an error occured.
void savePNG(char *filename, SDL_Surface *surface);

//Save a SDL_Surface to the path filename with a jpg format.
//Return NULL if an error occured.
void saveJPG(char *filename, SDL_Surface *surface);

//Convert an SDL_Surface *surface to a new SDL_Texture *texture.
//return NULL if an error occured.
SDL_Texture *surface_to_texture(SDL_Surface *surface, SDL_Renderer *renderer);

//Convert an SDL_Texture *texture to a new SDL_Surface *surface.
//return NULL if an error occured.
SDL_Surface *texture_to_surface(SDL_Texture *texture, SDL_Renderer *renderer);

//Print the SDL_Texture *texture to the window linked to SDL_Renderer *renderer at position posx and posy.
void print_texture_to_window(SDL_Texture *texture, SDL_Renderer *renderer);

//Get the pixel at position posx and posy in the SDL_Surface *surface.
//Return -1 if posx or posy are outside the image.
Uint32 get_pixel(SDL_Surface *surface, size_t posx, size_t posy);

//Set the pixel at the position posx and posy to the RGBA color r, g, b, a.
//It is mandatory that you lock the surface before using this function.
//Do nothing if posx or posy are outside the image.
void set_pixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, size_t posx, size_t posy);

//Initialize SDL and create :
//a new window with size w and h to the pointer **window
//a new renderer liked to **window to the pointer **renderer
//Clear everything if it fail.
void init(SDL_Window **window, SDL_Renderer **renderer, size_t w, size_t h);

//Destroy all parameters, put a NULL as parameters if nothing to destroy.
//Quit SDL.
void quit(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

void copy_surface(SDL_Surface *surface1, SDL_Surface *surface2);

#endif
