#include "color.h"

void brightest_pixel(SDL_Surface *surface, Uint8 *reference)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in brightest_pixel");
        return;
    }

    Uint8 max = 0;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 average = (r + g + b) / 3;

            if (average < 250 && average > max)
            {
                max = average;
                reference[0] = r;
                reference[1] = g;
                reference[2] = b;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void white_balance(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    Uint8 color[3];
    if (factor < -1 || factor > 1 || factor == 0)
        return;

    if (factor < 0)
    {
        color[0] = 0;
        color[1] = 67;
        color[2] = 255;
    }

    else
    {
        color[0] = 255;
        color[1] = 138;
        color[2] = 0;
    }

    factor = fabs(factor);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            double old_hsl[3];
            double new_hsl[3];
            Uint8 rgb[3];

            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsl(r, g, b, old_hsl);
            r = (r * (color[0] + (255 - color[0]) * (1 - factor))) / 255;
            g = (g * (color[1] + (255 - color[1]) * (1 - factor))) / 255;
            b = (b * (color[2] + (255 - color[2]) * (1 - factor))) / 255;

            rgb_to_hsl(r, g, b, new_hsl);
            new_hsl[2] = old_hsl[2];
            hsl_to_rgb(new_hsl[0], new_hsl[1], new_hsl[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void tint(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    Uint8 color[3];
    if (factor < -1 || factor > 1 || factor == 0)
        return;

    if (factor < 0)
    {
        color[0] = 0;
        color[1] = 203;
        color[2] = 0;
    }

    else
    {
        color[0] = 203;
        color[1] = 0;
        color[2] = 203;
    }

    factor = fabs(factor);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            double old_hsl[3];
            double new_hsl[3];
            Uint8 rgb[3];

            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsl(r, g, b, old_hsl);
            r = (r * (color[0] + (255 - color[0]) * (1 - factor))) / 255;
            g = (g * (color[1] + (255 - color[1]) * (1 - factor))) / 255;
            b = (b * (color[2] + (255 - color[2]) * (1 - factor))) / 255;

            rgb_to_hsl(r, g, b, new_hsl);
            new_hsl[2] = old_hsl[2];
            hsl_to_rgb(new_hsl[0], new_hsl[1], new_hsl[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}


void apply_lut(SDL_Surface *surface, SDL_Surface *lut)
{
    int image_h = surface->h;
    int image_w = surface->w;
    Uint8 s_r, s_g, s_b, s_a;
    Uint32 source_pixel;
    Uint8 l_r, l_g, l_b, l_a;
    Uint32 lut_pixel;
    Uint32 lut_x, lut_y;

    for (int j = 0; j < image_h; j++)
    {
        for (int i = 0; i < image_w; i++)
        {
            source_pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(source_pixel, surface->format, &s_r, &s_g, &s_b, &s_a);
            s_r /= 4;
            s_g /= 4;
            s_b /= 4;
            lut_x = ((s_b%8)*64) + s_r;
            lut_y = ((s_b/8)*64) + s_g;

            lut_pixel = get_pixel(lut, lut_x, lut_y);
            SDL_GetRGBA(lut_pixel, lut->format, &l_r, &l_g, &l_b, &l_a);
            set_pixel(surface, l_r, l_g, l_b, s_a, i, j);
        }
        
    }
    
}


void update_gradient_preview(int ra, int ga, int ba, int rb, int gb, int bb)
{
    SDL_Surface* gradient = create_surface(256, 40);
    int a_factor = 255;
    int b_factor = 0;

    Uint8 r, g, b, a;
    a = 1;

    for (int i = 0; i < 256; i++)
    {
        r = (((a_factor - i)*ra) + ((b_factor + i)*rb))/255;
        g = (((a_factor - i)*ga) + ((b_factor + i)*gb))/255;
        b = (((a_factor - i)*ba) + ((b_factor + i)*bb))/255;

        for (int j = 0; j < 40; j++)
        {
            set_pixel(gradient, r, g, b, a, i, j);
        }
        
    }
    
    savePNG("./gradient.png", gradient);
    SDL_FreeSurface(gradient);

}

void gradient_colorize(SDL_Surface *surface, 
        int ra, int ga, int ba, int rb, int gb, int bb)
{
    int image_h = surface->h;
    int image_w = surface->w;
    Uint8 s_r, s_g, s_b, s_a;
    Uint32 source_pixel;

    int a_factor = 255;
    int b_factor = 0;

    Uint8 r, g, b;


    for (int j = 0; j < image_h; j++)
    {
        for (int i = 0; i < image_w; i++)
        {
            source_pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(source_pixel, surface->format, &s_r, &s_g, &s_b, &s_a);
            a_factor = s_r;
            b_factor = 255 - s_r;

            r = (((a_factor)*ra) + ((b_factor)*rb))/255;
            g = (((a_factor)*ga) + ((b_factor)*gb))/255;
            b = (((a_factor)*ba) + ((b_factor)*bb))/255;

            set_pixel(surface, r, g, b, s_a, i, j);
        }
        
    }

}





void three_input_gradient_preview
    (int ra, int ga, int ba, int rb, int gb, int bb,
    int rc, int gc, int bc, char *path)
{
    SDL_Surface* gradient = create_surface(256, 40);
    int a_factor = 255;
    int b_factor = 0;

    Uint8 r, g, b, a;
    a = 1;

    for (int i = 0; i < 128; i++)
    {
        r = (((a_factor - i*2)*ra) + ((b_factor + i*2)*rb))/255;
        g = (((a_factor - i*2)*ga) + ((b_factor + i*2)*gb))/255;
        b = (((a_factor - i*2)*ba) + ((b_factor + i*2)*bb))/255;

        for (int j = 0; j < 40; j++)
        {
            set_pixel(gradient, r, g, b, a, i, j);
        }
        
    }

    for (int i = 128; i < 256; i++)
    {
        r = (((a_factor - ((i-128)*2))*rb) + ((b_factor + ((i-128)*2))*rc))/255;
        g = (((a_factor - ((i-128)*2))*gb) + ((b_factor + ((i-128)*2))*gc))/255;
        b = (((a_factor - ((i-128)*2))*bb) + ((b_factor + ((i-128)*2))*bc))/255;

        for (int j = 0; j < 40; j++)
        {
            set_pixel(gradient, r, g, b, a, i, j);
        }
        
    }
    
    savePNG(path, gradient);
    SDL_FreeSurface(gradient);

}



void three_input_gradient_colorize
    (SDL_Surface *surface, int ra, int ga, int ba, int rb, int gb, int bb,
    int rc, int gc, int bc)
{
    int image_h = surface->h;
    int image_w = surface->w;
    Uint8 s_r, s_g, s_b, s_a;
    Uint32 source_pixel;

    int a_factor = 255;
    int b_factor = 0;

    Uint8 r, g, b;


    for (int j = 0; j < image_h; j++)
    {
        for (int i = 0; i < image_w; i++)
        {
            source_pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(source_pixel, surface->format, &s_r, &s_g, &s_b, &s_a);
            a_factor = 255;

            if (s_r < 128)
            {
                r = (((a_factor - s_r*2)*ra) + ((b_factor + s_r*2)*rb))/255;
                g = (((a_factor - s_r*2)*ga) + ((b_factor + s_r*2)*gb))/255;
                b = (((a_factor - s_r*2)*ba) + ((b_factor + s_r*2)*bb))/255;
            }

            else
            {
                r = (((a_factor - ((s_r-128)*2))*rb) + ((b_factor + ((s_r-128)*2))*rc))/255;
                g = (((a_factor - ((s_r-128)*2))*gb) + ((b_factor + ((s_r-128)*2))*gc))/255;
                b = (((a_factor - ((s_r-128)*2))*bb) + ((b_factor + ((s_r-128)*2))*bc))/255;
            }
            
            /* r = (((a_factor)*ra) + ((b_factor)*rb))/255;
            g = (((a_factor)*ga) + ((b_factor)*gb))/255;
            b = (((a_factor)*ba) + ((b_factor)*bb))/255; */

            set_pixel(surface, r, g, b, s_a, i, j);
        }
        
    }

}