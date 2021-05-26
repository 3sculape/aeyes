#include "bilateral_filter.h"

/*double distance(int x, int y, int i, int j)
{
    return gsl_hypot(x - i, y - j);
}*/

double gaussian(float x, double sigma)
{
    return exp(-(pow(x, 2))/(2 * pow(sigma, 2))) / (2 * M_PI * pow(sigma, 2));
}

void filter(SDL_Surface* org, SDL_Surface* filtered, int x, int y, int
    diameter, double sigmaI, double sigmaS)
{
    double iFr = 0;
    double iFg = 0;
    double iFb = 0;
    double wP = 0;
    int half = diameter / 2;
    Uint8 a;

    for (int i = 0; i < diameter; ++i)
    {
        for (int j = 0; j < diameter; ++j)
        {
            int neighbor_x = x - (half - i);
            int neighbor_y = y - (half - j);
            Uint8 neighbor_r, neighbor_g , neighbor_b;
            Uint8 r, g, b;
            Uint32 pixel_neighbor = get_pixel(org, neighbor_x, neighbor_y);
            Uint32 pixel = get_pixel(org, x, y);
            SDL_GetRGBA(pixel, org->format, &r, &g, &b, &a);
            SDL_GetRGB(pixel_neighbor, org->format, &neighbor_r, &neighbor_g,
                       &neighbor_b);
            double gi = gaussian(neighbor_r - r, sigmaI);
            double gs = gaussian(distance(x, y, neighbor_x, neighbor_y),
                                 sigmaS);
            double w = gi * gs;
            iFr += neighbor_r * w;
            iFg += neighbor_g * w;
            iFb += neighbor_b * w;
            wP += w;
        }
    }
    iFr /= wP;
    iFg /= wP;
    iFb /= wP;
    set_pixel(filtered, iFr, iFg, iFb, a, x, y);
}

void bilateral_filter(SDL_Surface* original, double diameter, double sigmaI,
                      double sigmaS)
{
    SDL_Surface *filtered = create_surface(original->w, original->h);
    for (int i = 0; i < original->w; ++i)
    {
        for (int j = 0; j < original->h; ++j)
        {
            filter(original, filtered, i, j, diameter, sigmaI, sigmaS);
        }
    }
    copy_surface(filtered, original);
    SDL_FreeSurface(filtered);
}
