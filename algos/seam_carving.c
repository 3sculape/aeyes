#include "seam_carving.h"

void power_map(SDL_Surface *surface, SDL_Surface *edge_map)
{
    int image_h = surface->h;
    int image_w = surface->w;

    SDL_Surface* power_map = create_surface(image_w, image_h);

    Uint8 sr, sg, sb, sa;
    Uint32 s_pixel;

    Uint8 r1, g1, b1, a1;
    Uint32 pixel1;
    Uint8 r2, g2, b2, a2;
    Uint32 pixel2;
    Uint8 r3, g3, b3, a3;
    Uint32 pixel3;

    Uint8 pr, pg, pb, pa;
    Uint32 p_pixel;

    Uint8 min = 0;

    for (int j = image_h; j >= 0; j--)
    {
        for (int i = 0; i < image_w; i++)
        {
            s_pixel = get_pixel(edge_map, i, j);
            SDL_GetRGBA(s_pixel, edge_map->format, &sr, &sg, &sb, &sa);

            if (j == image_h)  //first row of power init
            {
                set_pixel(power_map, sr, sg, sb, sa, i, j);
            }

            if (j < image_h)
            {
                if (i > 0 && i < image_w)
                {
                    pixel1 = get_pixel(power_map, i-1, j+1);
                    SDL_GetRGBA(pixel1, power_map->format, &r1, &g1, &b1, &a1);
                    pixel2 = get_pixel(power_map, i, j+1);
                    SDL_GetRGBA(pixel2, power_map->format, &r2, &g2, &b2, &a2);
                    pixel3 = get_pixel(power_map, i+1, j+1);
                    SDL_GetRGBA(pixel3, power_map->format, &r3, &g3, &b3, &a3);

                    min = (min_three(r1, r2, r3));

                    p_pixel = get_pixel(power_map, i, j+1);
                    SDL_GetRGBA(p_pixel, power_map->format, &pr, &pg, &pb, &pa);

                    set_pixel(power_map, min+sr, min+sg, min+sb, sa, i, j);
                }
            }
        }
        
    }

    savePNG("./power_map.png", power_map);
    SDL_FreeSurface(power_map);
    
}




int min_three(int a, int b, int c)
{
    if (a > b)
    {
        if (c > a)
            return b;
        if (c > b)
            return b;
        return c;
    }

    else
    {
        if (c > b)
            return a;
        if (c > a)
            return a;
        return c;
    }
}

int min_two(int a, int b)
{
    if(a > b)
        return b;
    return a;
}