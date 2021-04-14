#include "canny_edge_detector.h"

void convolve_around_pixel(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker)
{
    // Variable definitions
    double nr;
    Uint8 a = 0;
    nr = 0;

    // Going through the kernel
    for (int offsetx = -1; offsetx < 2; offsetx++)
    {
        for (int offsety = -1; offsety < 2; offsety++)
        {
            Uint8 r, g, b;
            double kernelvalue = gsl_matrix_get(ker, 1 + offsetx, 1 + offsety);

            Uint32 pixel = get_pixel(surface, x + offsety, y + offsetx);
            if (pixel == (Uint32) -1)
            {
                set_pixel(copy, 0, 0, 0, a, x, y);
                return;
            }
            else
            {
                // Simple sum in function of neighbors with kernel
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                double tmpr = kernelvalue * (double)r;
                nr += tmpr;
            }
        }
    }
    nr = fabs(nr);
    set_pixel(copy, (Uint8)nr, (Uint8)nr, (Uint8)nr, a, x, y);
}

SDL_Surface* canny_fnc(SDL_Surface *surface)
{
    // Defining temporary surfaces to work on
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    SDL_Surface* canny = create_surface(surface->w, surface->h);
    copy_surface(surface, copy);
    // Preprocessing (noise reduction)
    grayscale(copy);
    gaussian_blur(copy, 5, 5);

    // Definition of Gradient kernels
    gsl_matrix* Gx = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gx, 0, 0, -1);
    gsl_matrix_set(Gx, 0, 1, 0);
    gsl_matrix_set(Gx, 0, 2, 1);
    gsl_matrix_set(Gx, 1, 0, -2);
    gsl_matrix_set(Gx, 1, 1, 0);
    gsl_matrix_set(Gx, 1, 2, 2);
    gsl_matrix_set(Gx, 2, 0, -1);
    gsl_matrix_set(Gx, 2, 1, 0);
    gsl_matrix_set(Gx, 2, 2, 1);
    SDL_Surface* copyGx = create_surface(surface->w, surface->h);
    // Convolution
    for(int i = 0; i < copy->w; i++)
    {
        for (int j = 0; j < copy -> h; j++)
        {
            convolve_around_pixel(copy, copyGx, i, j, Gx);
        }
    }

    gsl_matrix* Gy = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gy, 0, 0, 1);
    gsl_matrix_set(Gy, 0, 1, 2);
    gsl_matrix_set(Gy, 0, 2, 1);
    gsl_matrix_set(Gy, 1, 0, 0);
    gsl_matrix_set(Gy, 1, 1, 0);
    gsl_matrix_set(Gy, 1, 2, 0);
    gsl_matrix_set(Gy, 2, 0, -1);
    gsl_matrix_set(Gy, 2, 1, -2);
    gsl_matrix_set(Gy, 2, 2, -1);
    SDL_Surface* copyGy = create_surface(surface->w, surface->h);
    for(int i = 0; i < copy->w; i++)
    {
        for (int j = 0; j < copy -> h; j++)
        {
            convolve_around_pixel(copy, copyGy, i, j, Gy);
        }
    }

    savePNG("gx.PNG", copyGx);
    savePNG("gy.PNG", copyGy);

    // Computing sqrt(Gx² + Gy²) and Theta = arctan(Gy/Gx)
    gsl_matrix* theta = gsl_matrix_calloc(surface->w - 1, surface->h - 1);
    for(int i = 1; i < canny -> w - 1; i++)
    {
        if (i == 2)
            break;
        for (int j = 1; j < canny -> h - 1; j++)
        {
            Uint8 a;
            Uint8 xr, xg, xb;
            Uint8 yr, yg, yb;
            Uint32 pixelx = get_pixel(copyGx, i, j);
            Uint32 pixely = get_pixel(copyGy, i, j);

            SDL_GetRGBA(pixelx, copyGx -> format, &xr, &xg, &xb, &a);
            SDL_GetRGB(pixely, copyGy -> format, &yr, &yg, &yb);
            double nr = gsl_hypot((double)xr, (double)yr);
            double ng = gsl_hypot((double)xg, (double)yg);
            double nb = gsl_hypot((double)xb, (double)yb);
            set_pixel(canny, (Uint8)nr, (Uint8)ng, (Uint8)nb, a, i, j);
            double tmpx = (double)xr;
            double tmpy = (double)yr;
            gsl_matrix_set(theta, i, j, atan((tmpy / tmpx)));
        }
    }

    for (int i = 1; i < canny->w - 1; i++)
    {
        for (int j = 1; j < canny->h - 1; j++)
        {
            Uint8 qr, qg, qb;
            Uint8 rr, rg, rb;
            if (theta[i-1, j-1] < 22.5 ||
                    (theta[i-1, j-1] >=157.5 && theta[i-1, j-1] <= 180))
            {

            }
            else if (theta[i-1, j-1] >= 22.5 && theta[i-1, j-1] < 67.5)
            {

            }
            else if (theta[i-1, j-1] >= 67.5  && theta[i-1, j-1] <= 112.5)
            {

            }
            else if (theta[i-1, j-1] < 112.5 || && theta[i-1, j-1] <= 157.5)
            {

            }
        }
    }

    // Memory cleanup
    gsl_matrix_free(Gx);
    gsl_matrix_free(Gy);
    gsl_matrix_free(theta);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(copyGx);
    SDL_FreeSurface(copyGy);
    return canny;
}
