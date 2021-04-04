#include <err.h>
#include "lib_sdl.h"
#include <SDL2/SDL_image.h>
#include <png.h>
#include <jpeglib.h>
#include <stdio.h>

void create_window(size_t width, size_t height, SDL_Window **window, SDL_Renderer **renderer)
{
    SDL_CreateWindowAndRenderer(width,
            height,
            SDL_WINDOW_HIDDEN,
            window,
            renderer
            );
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

void savePNG(char *filename, SDL_Surface *surface)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int x, y;
    png_byte ** row_pointers = NULL;
    int pixel_size = 3;
    int depth = 8;

    fp = fopen(filename, "wb");
    if (!fp)
    {
        return;
    }

    png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            NULL,
            NULL
            );
    if (!png_ptr)
        return;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(
                &png_ptr,
                NULL
                );
        return;
    }

    png_set_IHDR (png_ptr,
            info_ptr,
            surface->w,
            surface->h,
            depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
            );

    row_pointers = png_malloc (png_ptr, surface->h * sizeof (png_byte *));
    for (y = 0; y < surface->h; y++) {
        png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * surface->w * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < surface->w; x++) {
            Uint32 pixel = get_pixel(surface, x, y);
            uint8_t r, g, b, a;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            *row++ = (uint8_t)r;
            *row++ = (uint8_t)g;
            *row++ = (uint8_t)b;
        }
    }

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);


    for (y = 0; y < surface->h; y++) {
        png_free (png_ptr, row_pointers[y]);
    }

    png_free (png_ptr, row_pointers);
    png_destroy_write_struct (&png_ptr, &info_ptr);
    fclose (fp);
    return;
}

void saveJPG(char *filename, SDL_Surface *surface)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    FILE *outfile = fopen( filename, "wb" );

    if ( !outfile )
    {
        printf("Error opening output jpeg file %s\n!", filename );
        return;
    }

    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);


    cinfo.image_width = surface->w;
    cinfo.image_height = surface->h;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults( &cinfo );

    jpeg_start_compress( &cinfo, TRUE );

    unsigned char *raw_image = (unsigned char*)malloc( surface->w * surface->h * cinfo.num_components );

    for(int y = 0; y < surface->h; y++)
    {
        for(int x = 0; x < surface->w; x++)
        {
            Uint32 pixel = get_pixel(surface, x, y);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel,surface->format, &r, &g, &b, &a);

            raw_image[y * surface->w * 3 + x * 3] = (unsigned char)r;
            raw_image[y * surface->w * 3 + x * 3 + 1] = (unsigned char)g;
            raw_image[y * surface->w * 3 + x * 3 + 2] = (unsigned char)b;
        }
    }

    while( cinfo.next_scanline < cinfo.image_height )
    {
        row_pointer[0] = &raw_image[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
        jpeg_write_scanlines( &cinfo, row_pointer, 1 );
    }

    free(raw_image);
    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose( outfile );

    return;
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


SDL_Surface *texture_to_surface(SDL_Texture *texture, SDL_Renderer *renderer)
{
	SDL_Surface		*surface;
	int				w, h;
	if(SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
        return NULL;

	Uint32 *pixels = malloc(w * h * sizeof(Uint32));
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, pixels, w * sizeof(Uint32));
    SDL_SetRenderTarget(renderer, NULL);

    if(pixels == NULL)
    {
        warnx("%s\n", SDL_GetError());
    }

	surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);

    //SDL_LockSurface(surface);

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = pixels[j * surface->w + i];
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    //SDL_UnlockSurface(surface);
    free(pixels);

	return surface;
}

void print_texture_to_window(SDL_Texture *texture, SDL_Renderer *renderer)
{
    SDL_QueryTexture(texture, NULL, NULL, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
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


    create_window(w, h, window, renderer);
    if(*window == NULL || *renderer == NULL)
    {
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

void copy_surface(SDL_Surface *surface1, SDL_Surface *surface2)
{
    for(int i = 0; i < surface1->w; i++)
    {
        for(int j = 0; j < surface1->h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface1, i, j);
            SDL_GetRGBA(pixel, surface1 -> format, &r, &g, &b, &a);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
}
