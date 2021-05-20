#include <stdio.h>
#include <stdlib.h>
#include "color_histogram.h"
#include <err.h>

#define COEFF 2.55
#define LEN 101

SDL_Surface* resize2(SDL_Surface *surface, double factor)
{
    return rotozoomSurface(surface, 0, factor, 1);
}

void clrl_histo(SDL_Surface *surface, int red[], int green[], int blue[])
{
    int copy[LEN] = {0};
    int w = surface->w;
    int h = surface->h;

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            double hsl[3];
            rgb_to_hsl(r, g, b, hsl);
            size_t l = (size_t)hsl[2];
            copy[l] += 1;
            red[l] += r;
            green[l] += g;
            blue[l] += b;
        }
    }
    for (int i = 0; i < LEN; i++)
    {
        if (copy[i])
        {
            red[i] /= copy[i];
            green[i] /= copy[i];
            blue[i] /= copy[i];
        }
    }
}
void l_histo(SDL_Surface *surface, int lum[])
{
    int w = surface->w;
    int h = surface->h;

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            double hsl[3];
            rgb_to_hsl(r, g, b, hsl);
            int l = (int)hsl[2];
            // printf("%i\n", l);
            lum[l] += 1;
        }
    }
    /*printf("[ %i", lum[0]);
    for (int i = 1; i < LEN; i++)
    {
        printf(", %i", lum[i]);
    }
    printf("]\n");*/

}

int get_max(int arr[])
{
    int res = arr[0];
    for(int i = 1; i < LEN; i++)
    {
        if (arr[i] > res)
            res = arr[i];
    }
    return res;
}

void show_l_histo(SDL_Surface * surface)
{
    SDL_Surface* histo = create_surface(LEN, LEN);
    double factor = 100.0 / (double)surface->w;
    SDL_Surface* tmp = resize2(surface, factor);
    int lum[LEN] = {0};
    l_histo(surface, lum);/*
    printf("[ %i", lum[0]);
    for (int i = 1; i < LEN; i++)
    {
        printf(", %i", lum[i]);
    }
    printf("]\n");*/
    SDL_FreeSurface(tmp);
    int range = get_max(lum);
    for(int i = 0; i < LEN; i++)
    {
        int tmp = lum[i] / (int)((double)range / 100.0);
        for (int j = LEN; j > 0; --j)
        {
            if (tmp)
            {
                set_pixel(histo, 255,255,255, 1, i, j);
                tmp -= 1;
            }
            else
            {
                set_pixel(histo, 0, 0, 0, 0, i, j);
            }
        }
    }
    savePNG("./l_histo.PNG", histo);
    SDL_FreeSurface(histo);
}

void show_clrl_histo(SDL_Surface* surface)
{
    SDL_Surface* histo = create_surface(LEN, LEN);
    double factor = 100.0 / (double)surface->w;
    SDL_Surface* tmp = resize2(surface, factor);
    int red[LEN] = {0};
    int green[LEN] = {0};
    int blue[LEN] = {0};
    clrl_histo(tmp, red, green, blue);
    SDL_FreeSurface(tmp);

    for(int i = 0; i < LEN; i++)
    {
        int ri = (int)((double)red[i] / COEFF);
        int gi = (int)((double)green[i] / COEFF);
        int bi = (int)((double)blue[i] / COEFF);
        int j = 100;
        if (!ri && !gi && !bi)
        {
            for(; j > 0; --j)
            {
                set_pixel(histo, 0, 0, 0, 0, i, j);
            }
            continue;
        }
        for(; j > 0; --j)
        {
            Uint8 r, g, b, a;
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            if (ri)
            {
                r = 255;
                ri -= 1;
            }
            if (gi)
            {
                g = 255;
                gi -= 1;
            }
            if (bi)
            {
                b = 255;
                bi -= 1;
            }
            a = r || g || b;
            set_pixel(histo, r, g, b, a, i, j);
        }
    }

    savePNG("histo.PNG", histo);
    SDL_FreeSurface(histo);
}

void color_histogram(SDL_Surface* surface, size_t red[], size_t green[],
        size_t blue[])
{
    size_t maxh = surface->h;
    size_t maxw = surface->w;
    for(size_t width = 0; width < maxw; width++)
    {
        Uint8        r, g, b;
        size_t       avgr = 0;
        size_t       avgg = 0;
        size_t       avgb = 0;

        for(size_t height = 0; height < maxh; height++)
        {
            Uint32  pixel;
            /*if ((int)*/(pixel = get_pixel(surface, width, height));/* == -1)
                errx(EXIT_FAILURE, "get_pixel returned -1");*/
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            avgr += (size_t) r;
            avgg += (size_t) g;
            avgb += (size_t) b;
        }
        avgr /= maxh;
        avgg /= maxh;
        avgb /= maxh;
        red[width] = avgr;
        green[width] = avgg;
        blue[width] = avgb;
    }
}

SDL_Surface* show_histogram(SDL_Surface *surface)
{
    // Creates the output image
    SDL_Surface* histo = create_surface(250, 100);

    // calculates the factor in function of the image size
    double factor = 250.0 / (double)surface->w;
    //Temporary image of the size of the box
    SDL_Surface* tmp = resize2(surface, factor);
    size_t red[250];
    size_t green[250];
    size_t blue[250];
    //create histo
    color_histogram(tmp, red, green, blue);
    SDL_FreeSurface(tmp);
    for(int i = 0; i < 250; i++)
    {
        int j = 100;
        // scales the rgb values from 255 to 100
        int ri = (int)(COEFF * (double)red[i]);
        int gi = (int)(COEFF * (double)green[i]);
        int bi = (int)(COEFF * (double)blue[i]);

        if (!ri && !gi && !bi)
        {
            for(; j > 0; --j)
            {
                set_pixel(histo, 0, 0, 0, 0, i, j);
            }
            continue;
        }
        for(; j > 0; --j)
        {
            Uint8 r, g, b, a;
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            if (ri)
            {
                r = 255;
                ri -= 1;
            }
            if (gi)
            {
                g = 255;
                gi -= 1;
            }
            if (bi)
            {
                b = 255;
                bi -= 1;
            }
            a = r || g || b;
            set_pixel(histo, r, g, b, a, i, j);
        }
    }
    return histo;
}


int three_max(int a, int b, int c) {
    if(a <= b) {
        if(b <= c) return c;
        return b;
    }
    return a <= c? c : a;
}

int color_get_max(int arr[])
{
    int res = arr[0];
    for(int i = 1; i < 255; i++)
    {
        if (arr[i] > res)
            res = arr[i];
    }
    return res;
}


void histo_color(SDL_Surface * surface)
{
    SDL_Surface* new_histo = create_surface(256, LEN);
    int main_w = surface->w;
    int main_h = surface->h;

    int total_r[256] = {0};
    int total_g[256] = {0};
    int total_b[256] = {0};

    for (int i = 0; i < main_w; i++)
    {
        for (int j = 0; j < main_h; j++)
        {
            Uint32 pixel = get_pixel(surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            total_r[r] += 1;
            total_g[g] += 1;
            total_b[b] += 1;
        }
        
    }

    int max_r = color_get_max(total_r);
    int max_g = color_get_max(total_g);
    int max_b = color_get_max(total_b);

    int global_max = three_max(max_r, max_g, max_b);

    float resize_factor = global_max/100;

    int histo_w = 256;

    int col_h_r = 0;
    int col_h_g = 0;
    int col_h_b = 0;

    int start_r = 100;
    int start_g = 100;
    int start_b = 100;

    for (int x = 0; x < histo_w; x++)
    {
        col_h_r = (int)((float)total_r[x]/resize_factor);
        col_h_g = (int)((float)total_g[x]/resize_factor);
        col_h_b = (int)((float)total_b[x]/resize_factor);

        start_r = 100;
        start_g = 100;
        start_b = 100;

        while (col_h_r != 0)
        {
            set_pixel(new_histo, 255, 0, 0, 1, x, start_r);
            start_r -= 1;
            col_h_r -= 1;
        }

        while (col_h_g != 0)
        {
            Uint32 red_pixel = get_pixel(new_histo, x, start_g);
            Uint8 red_r, red_g, red_b;
            SDL_GetRGB(red_pixel, surface->format, &red_r, &red_g, &red_b);

            set_pixel(new_histo, red_r, red_g + 255, red_b + 0, 1, x, start_g);
            start_g -= 1;
            col_h_g -= 1;
        }

        while (col_h_b != 0)
        {
            Uint32 green_pixel = get_pixel(new_histo, x, start_b);
            Uint8 green_r, green_g, green_b;
            SDL_GetRGB(green_pixel, surface->format, &green_r,
                &green_g, &green_b);

            set_pixel(new_histo, green_r, green_g, green_b + 255, 1,
                x, start_b);
            
            start_b -= 1;
            col_h_b -= 1;
        }
        
    }
    
    savePNG("./new_histo.PNG", new_histo);
    SDL_FreeSurface(new_histo);
}


int histo_clipping(SDL_Surface *histogram)
{
    int image_h = histogram->h;
    int image_w = histogram->w;

    Uint8 r1, g1, b1, a1;
    Uint32 pixel1;

    Uint8 r2, g2, b2, a2;
    Uint32 pixel2;

    pixel1 = get_pixel(histogram, 0, image_h-1);
    SDL_GetRGBA(pixel1, histogram->format, &r1, &g1, &b1, &a1);
    pixel2 = get_pixel(histogram, image_w-1, image_h-1);
    SDL_GetRGBA(pixel2, histogram->format, &r2, &g2, &b2, &a2);

    int res = 0;

    if((r1 + g1 + b1) != 0)
    {
        res+= 1;
    }

    if((r2 + g2 + b2) != 0)
    {
        res+= 2;
    }

    return res;
}