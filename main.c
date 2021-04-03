#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <stdio.h>
#include "utils/lib_sdl.h"
#include "algos/basic.h"
#include "algos/convert.h"
#include "algos/color.h"
#include "utils/lib_exif.h"
#include "algos/unsharp_masking.h"
#include "algos/color_histogram.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(EXIT_FAILURE, "error ");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Renderer* renderer;
    SDL_Window* window;
    create_window(500, 500, &window, &renderer);
    SDL_Surface* original = load(argv[1]);
    //savePNG("original.PNG", original);
 /*   // saveJPG("test.JPG", original);
    if (original == NULL)
        errx(EXIT_FAILURE, "Load image returned NULL");
    SDL_Surface* sharpened01 = sharpen(original, 0.1, 50);
    SDL_Surface* sharpened05 = sharpen(original, 0.5, 50);
    SDL_Surface* sharpened1 = sharpen(original, 1, 50);
    SDL_Surface* sharpened2 = sharpen(original, 2, 50);
    SDL_Surface* sharpened5 = sharpen(original, 5, 50);
    if (sharpened == NULL)
        errx(EXIT_FAILURE, "unsharp_masking returned NULL");
    savePNG("sharpened01.PNG", sharpened01);
    savePNG("sharpened05.PNG", sharpened05);
    savePNG("sharpened1.PNG", sharpened1);
    savePNG("sharpened2.PNG", sharpened2);
    savePNG("sharpened5.PNG", sharpened5);
    SDL_FreeSurface(original);
    SDL_FreeSurface(sharpened01);
    SDL_FreeSurface(sharpened05);
    SDL_FreeSurface(sharpened1);
    SDL_FreeSurface(sharpened2);
    SDL_FreeSurface(sharpened5);
    quit(window, renderer, NULL);*/
    show_l_histo(original);
    SDL_FreeSurface(original);
    return 0;
}
