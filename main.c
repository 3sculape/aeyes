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
#include "algos/blurs.h"

SDL_Window *sdl_window;
SDL_Renderer *sdl_renderer;
SDL_Texture *texture;
GtkWindow *gtk_window;
GtkBuilder *builder;
GError* error;
GtkDrawingArea *gtk_da;
void *gdk_window;
void *window_id;

int resizingEventWatcher(void *data, SDL_Event* event) 
{
    if (event->type == SDL_WINDOWEVENT) 
    {
        SDL_RenderClear(sdl_renderer);
        SDL_RenderCopy(sdl_renderer, texture, NULL, NULL);
        SDL_RenderPresent(sdl_renderer);
    }
    return 0;
}

gboolean on_configure()
{
    SDL_Event user_event;

    user_event.type = SDL_WINDOWEVENT;
    user_event.user.code = 2;
    SDL_PushEvent(&user_event);

    return True;
}

gboolean on_quit()
{
    gtk_main_quit();
    return True;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
        errx(3, "Usage : ./gtk <filename>");

    gtk_init(NULL, NULL);


    GtkBuilder* builder = gtk_builder_new();
    error = NULL;
    if (gtk_builder_add_from_file(builder, "test_ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    gtk_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));

    gtk_da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "image_window"));

    gtk_widget_show_all(GTK_WIDGET(gtk_window));
}
