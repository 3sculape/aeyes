#include "canny_edge_detector.h"

void convolve_around_pixel(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker)
{
    Uint8 nr, ng, nb, a;
    nr = 0;
    ng = 0;
    nb = 0;
//    printf("START POS : %i and %i\n", x, y);
    for (int offsetx = -1; offsetx < 2; offsetx++)
    {
        for (int offsety = -1; offsety < 2; offsety++)
        {
            Uint8 r, g, b;
            double kernelvalue = gsl_matrix_get(ker, 1 + offsetx, 1 + offsety);

            // Issue is that pixel got is not coherent with value in kernel (example : -1,0 takes 0 instead of -2)
            Uint32 pixel = get_pixel(surface, x + offsetx, y + offsety);
  //          printf("%f at pos %i, %i \n", kernelvalue, x + offsetx, y + offsety);
            if (pixel == (Uint32) -1)
            {
                //warnx("fail pixel");
                set_pixel(copy, 255, 0, 0, a, x, y);
                //return;
            }
            else
            {
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                double tmpr = kernelvalue * (double)r;
                double tmpg = kernelvalue * (double)g;
                double tmpb = kernelvalue * (double)b;
                nr += (Uint8)tmpr;
                ng += (Uint8)tmpg;
                nb += (Uint8)tmpb;
            }
        }
    }
    //errx(EXIT_FAILURE, "TEST");
    set_pixel(copy, nr, ng, nb, a, x, y);
}

SDL_Surface* canny_fnc(SDL_Surface *surface)
{
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    SDL_Surface* canny = create_surface(surface->w, surface->h);
    copy_surface(surface, copy);
    grayscale(copy);
    savePNG("gray.PNG", copy);
    gaussian_blur(copy, 3, 3);
    savePNG("blur.PNG", copy);

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
    negative(copyGx);
    negative(copyGy);

    savePNG("gx.PNG", copyGx);
    savePNG("gy.PNG", copyGy);

    for(int i = 0; i < canny -> w; i++)
    {
        for (int j = 0; j < canny -> h; j++)
        {
            Uint8 r, g, b, a;
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
        }
    }
    gsl_matrix_free(Gx);
    gsl_matrix_free(Gy);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(copyGx);
    SDL_FreeSurface(copyGy);
    return canny;
}
