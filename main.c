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

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(3, "Usage: ./gtk <JPEG image path>");
    show_info(argv[1]);
    return 0;
}
