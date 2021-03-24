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

    gdk_window = gtk_widget_get_window(GTK_WIDGET(gtk_da));
    window_id = (void*)(intptr_t)GDK_WINDOW_XID(gdk_window);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    SDL_Surface *surface;
    surface = load(argv[1]);
    if(surface == NULL)
    {
        errx(3, "Coudn't load %s", argv[1]);
    }

    sdl_window = SDL_CreateWindowFrom(window_id);
    if(!sdl_window)
        errx(1, "Coudn't create window");

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);

    motion_blur(surface, 31);
    saveJPG(argv[2], surface);

    texture = create_texture(sdl_renderer, surface->w, surface->h);
    texture = surface_to_texture(surface, sdl_renderer);

    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, texture, NULL, NULL);
    SDL_RenderPresent(sdl_renderer);

    SDL_FreeSurface(surface);

    SDL_AddEventWatch(resizingEventWatcher, NULL);

    g_signal_connect(gtk_window, "configure-event", G_CALLBACK(on_configure), NULL);
    g_signal_connect(gtk_window, "destroy", G_CALLBACK(on_quit), NULL);
    gtk_main();
    quit(sdl_window, sdl_renderer, texture);
    return 0;

}
