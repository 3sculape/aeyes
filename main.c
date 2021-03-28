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

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(3, "Usage: ./gtk <JPEG image path>");
    // show_info(argv[1]);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Renderer* renderer;
    SDL_Window* window;
    create_window(500, 500, &window, &renderer);
    SDL_Surface* original = load(argv[1]);
    // saveJPG("test.JPG", original);
    if (original == NULL)
        errx(EXIT_FAILURE, "Load image returned NULL");
    SDL_Surface* sharpened = sharpen(original, 0.65);
    if (sharpened == NULL)
        errx(EXIT_FAILURE, "unsharp_masking returned NULL");
    savePNG("result.PNG", sharpened);
    SDL_FreeSurface(original);
    SDL_FreeSurface(sharpened);
    quit(window, renderer, NULL);
    return 0;
}
