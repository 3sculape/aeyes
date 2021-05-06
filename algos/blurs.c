#include "../utils/lib_sdl.h"
#include "basic.h"
#include <SDL2/SDL2_rotozoom.h>
#include "blurs.h"
#include "utility.h"
#include <gsl/gsl_matrix.h>
#include <math.h>

typedef struct {
    int r[256], g[256], b[256];
    int n;
} color_histo_t;

void get_pixel_around_x(SDL_Surface *surface, Uint32 *matrix,
        int posx, int posy, int x)
{
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        for(int i = posx - (x - 1) / 2; i <= posx + (x - 1) / 2; i++)
        {
            if(j < 0)
            {
                if(i < 0)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i + (x - 1) / 2, j + (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i - (x - 1) / 2, j + (x - 1) / 2);
                }
                else
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i, j + (x - 1) / 2);
                }
            }

            else if(j >= surface->h)
            {
                if(i < 0)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i + (x - 1) / 2, j - (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i - (x - 1) / 2, j - (x - 1) / 2);
                }
                else
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i
                        - posx + (x - 1) / 2] =
                        get_pixel(surface, i, j - (x - 1) / 2);
                }
            }

            else if(i < 0)
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i + (x - 1) / 2, j);
            }
            else if(i >= surface->w)
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i - (x - 1) / 2, j);
            }
            else
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i, j);
            }
        }
    }
}

void get_pixel_rect(SDL_Surface *surface, Uint32 *rect, int posx, int posy,
        int x, int hor)
{
    if(hor)
    {
        for(int i = posx - (x - 1) / 2; i <= posx + (x - 1) / 2; i++)
        {
            if(i < 0)
            {
                rect[i - posx + (x - 1) / 2] = get_pixel(surface,
                        i + (x - 1) / 2, posy);
            }
            else if(posx >= surface->w)
            {
                rect[i - posx + (x - 1) / 2] = get_pixel(surface,
                        i - (x - 1) / 2, posy);
            }
            else
            {
                rect[i - posx + (x - 1) / 2] = get_pixel(surface, i, posy);
            }
        }
    }
    else
    {
        for(int i = posy - (x - 1) / 2; i <= posy + (x - 1) / 2; i++)
        {
            if(i < 0)
            {
                rect[i - posy + (x - 1) / 2] = get_pixel(surface, posx,
                        i + (x - 1) / 2);
            }
            else if(posy >= surface->h)
            {
                rect[i - posy + (x - 1) / 2] = get_pixel(surface, posx,
                        i - (x - 1) / 2);
            }
            else
            {
                rect[i - posy + (x - 1) / 2] = get_pixel(surface, posx,
                        i);
            }
        }
    }
}

void get_pixel_rect_angle(SDL_Surface *surface, Uint32 *rect, int posx,
        int posy, int x, double cosa, double sina)
{
    int w  = surface->w;
    int h = surface->h;

    for(int i = posx - (x - 1) / 2; i <= posx + (x - 1) / 2; i++)
    {
        int xt = i - posx;

        int xs = (int)round((cosa * xt) + posx);
        int ys = (int)round(((-sina) * xt) + posy);

        if(ys < 0)
        {
            if(xs < 0)
            {
                Uint32 pixel = get_pixel(surface, xs + (x - 1) / 2,
                        ys + (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
            else if(xs >= w)
            {
                Uint32 pixel = get_pixel(surface, xs - (x - 1) / 2,
                        ys + (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
            else
            {
                Uint32 pixel = get_pixel(surface, xs, ys + (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
        }

        else if(ys >= h)
        {
            if(xs < 0)
            {
                Uint32 pixel = get_pixel(surface, xs + (x - 1) / 2,
                        ys - (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
            else if(xs >= w)
            {
                Uint32 pixel = get_pixel(surface, xs - (x - 1) / 2,
                        ys - (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
            else
            {
                Uint32 pixel = get_pixel(surface, xs, ys - (x - 1) / 2);
                rect[i - posx + (x - 1) / 2] = pixel;
            }
        }

        else if(xs < 0)
        {
            Uint32 pixel = get_pixel(surface, xs + (x - 1) / 2, ys);
            rect[i - posx + (x - 1) / 2] = pixel;
        }
        else if(xs >= w)
        {
            Uint32 pixel = get_pixel(surface, xs - (x - 1) / 2, ys);
            rect[i - posx + (x - 1) / 2] = pixel;
        }
        else
        {
            Uint32 pixel = get_pixel(surface, xs, ys);
            rect[i - posx + (x - 1) / 2] = pixel;
        }
    }
}

void del_pixels(SDL_Surface *surface, int posx, int posy, int x,
        color_histo_t *h)
{
    if(posx < 0 || posx > surface->w) return;
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        if(j < 0 || j > surface->h) continue;
        Uint32 pixel = get_pixel(surface, posx, j);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        h->r[r]--;
        h->g[g]--;
        h->b[b]--;
        h->n--;
    }
}
void add_pixels(SDL_Surface *surface, int posx, int posy, int x,
        color_histo_t *h)
{
    if(posx < 0 || posx > surface->w) return;
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        if(j < 0 || j > surface->h) continue;
        Uint32 pixel = get_pixel(surface, posx, j);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        h->r[r]++;
        h->g[g]++;
        h->b[b]++;
        h->n++;
    }
}

void get_average(SDL_Surface *surface, Uint32 *matrix, Uint8 *r, Uint8 *g,
        Uint8 *b, Uint8 *a, int x)
{
    int totr, totg, totb;
    totr = 0;
    totg = 0;
    totb = 0;
    for (int i = 0; i < x * x; i++)
    {
        SDL_GetRGB(matrix[i], surface->format, r, g, b);
        totr += *r;
        totg += *g;
        totb += *b;
    }

    SDL_GetRGBA(matrix[(x - 1) / 2 * x + (x - 1) / 2], surface->format,
            r, g, b, a);

    totr /= x * x;
    totg /= x * x;
    totb /= x * x;

    *r = totr;
    *g = totg;
    *b = totb;
}

void get_average_rect(SDL_Surface *surface, Uint32 *rect, Uint8 *r, Uint8 *g,
        Uint8 *b, Uint8 *a, int x)
{
    int totr, totg, totb;
    totr = 0;
    totg = 0;
    totb = 0;
    for (int i = 0; i < x; i++)
    {
        SDL_GetRGB(rect[i], surface->format, r, g, b);
        totr += *r;
        totg += *g;
        totb += *b;
    }

    SDL_GetRGBA(rect[(x - 1) / 2], surface->format, r, g, b, a);

    totr /= x;
    totg /= x;
    totb /= x;

    *r = totr;
    *g = totg;
    *b = totb;
}

gsl_matrix *gaussian_filter(int x, double sigma, double *sum)
{
    gsl_matrix *matrix = gsl_matrix_calloc(x, x);
    *sum = 0;

    for(int j = -1 * (x - 1) / 2; j <= (x - 1) / 2; j++)
    {
        for(int i = -1 * (x - 1) / 2; i <= (x - 1) / 2; i++)
        {
            double d;
            d = exp(-1 * (double)(i * i + j * j) / (2 * sigma * sigma)) /
                sqrt(2 * sigma * sigma * M_PI);
            gsl_matrix_set(matrix, (size_t)(i + (x - 1) / 2),
                    (size_t)(j + (x - 1) / 2), d);
            *sum += d;
        }
    }

    return matrix;
}

gsl_vector *gaussian_filter_rect(int x, double sigma, double *sum)
{
    gsl_vector *rect = gsl_vector_calloc(x);
    *sum = 0;

    for(int i = -1 * (x - 1) / 2; i <= (x - 1) / 2; i++)
    {
        double d;
        d = exp(-1 * (double)(i * i) / (2 * sigma * sigma)) /
            (sqrt(2 * M_PI) * sigma);
        gsl_vector_set(rect, (size_t)(i + (x - 1) / 2), d);
        *sum += d;
    }

    return rect;
}

void gaussian_average(size_t x, double sum, Uint32 *matrix, gsl_matrix *filter,
        SDL_Surface *surface, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    double rtot = 0;
    double gtot = 0;
    double btot = 0;
    for(size_t j = 0; j < x; j++)
    {
        for(size_t i = 0; i < x; i++)
        {
            SDL_GetRGB(matrix[j * x + i], surface->format, r, g, b);
            rtot += gsl_matrix_get(filter, i, j) * (double)(*r);
            gtot += gsl_matrix_get(filter, i, j) * (double)(*g);
            btot += gsl_matrix_get(filter, i, j) * (double)(*b);
        }
    }

    SDL_GetRGBA(matrix[((x - 1) / 2) * x + (x - 1) / 2],
            surface->format, r, g, b, a);

    rtot /= sum;
    gtot /= sum;
    btot /= sum;

    *r = (Uint8)rtot;
    *g = (Uint8)gtot;
    *b = (Uint8)btot;
}

void gaussian_average_rect(size_t x, double sum, Uint32 *rect,
        gsl_vector *filter, SDL_Surface *surface, Uint8 *r, Uint8 *g, Uint8 *b,
        Uint8 *a)
{
    double rtot = 0;
    double gtot = 0;
    double btot = 0;
    for(size_t i = 0; i < x; i++)
    {
        SDL_GetRGB(rect[i], surface->format, r, g, b);
        rtot += gsl_vector_get(filter, i) * (double)(*r);
        gtot += gsl_vector_get(filter, i) * (double)(*g);
        btot += gsl_vector_get(filter, i) * (double)(*b);
    }

    SDL_GetRGBA(rect[(x - 1) / 2],
            surface->format, r, g, b, a);

    rtot = round(rtot / sum);
    gtot = round(gtot / sum);
    btot = round(btot / sum);

    *r = (Uint8)rtot;
    *g = (Uint8)gtot;
    *b = (Uint8)btot;
}

void init_histo(SDL_Surface *surface, int posy, int x, color_histo_t *h)
{
    memset(h, 0, sizeof(color_histo_t));
    for(int i = -1 * (x - 1) / 2; i <= (x - 1) / 2; i++)
        add_pixels(surface, i, posy, x, h);
}

int median_histo(const int *x, int n)
{
    int i;
    for (n /= 2, i = 0; i < 256 && (n -= x[i]) > 0; i++);
    return i;
}
int min_histo(const int *x)
{
    int i;
    for(i = 0; i < 256 && x[i] == 0; i++);
    return i;
}
int max_histo(const int *x)
{
    int i;
    for(i = 255; i >= 0 && x[i] == 0; i--);
    return i;
}

void median_color(Uint8 *r, Uint8 *g, Uint8 *b, const color_histo_t *h)
{
    *r = median_histo(h->r, h->n);
    *g = median_histo(h->g, h->n);
    *b = median_histo(h->b, h->n);
}

void min_color(Uint8 *r, Uint8 *g, Uint8 *b, const color_histo_t *h)
{
    *r = min_histo(h->r);
    *g = min_histo(h->g);
    *b = min_histo(h->b);
}

void max_color(Uint8 *r, Uint8 *g, Uint8 *b, const color_histo_t *h)
{
    *r = max_histo(h->r);
    *g = max_histo(h->g);
    *b = max_histo(h->b);
}

void box_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;

    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    Uint32 *matrix = (Uint32 *)malloc(sizeof(Uint32) * x * x);
    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_around_x(surface, matrix, i, j, x);
            get_average(surface, matrix, &r, &g, &b, &a, x);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    free(matrix);
    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void median_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);

    color_histo_t h;

    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            if(i == 0) init_histo(surface, j, x, &h);
            else
            {
                del_pixels(surface, i - (x - 1) / 2 - 1, j, x, &h);
                add_pixels(surface, i + (x - 1) / 2, j, x, &h);
            }
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            median_color(&r, &g, &b, &h);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void min_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);

    color_histo_t h;

    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            if(i == 0) init_histo(surface, j, x, &h);
            else
            {
                del_pixels(surface, i - (x - 1) / 2 - 1, j, x, &h);
                add_pixels(surface, i + (x - 1) / 2, j, x, &h);
            }
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            min_color(&r, &g, &b, &h);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void max_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);

    color_histo_t h;

    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            if(i == 0) init_histo(surface, j, x, &h);
            else
            {
                del_pixels(surface, i - (x - 1) / 2 - 1, j, x, &h);
                add_pixels(surface, i + (x - 1) / 2, j, x, &h);
            }
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            max_color(&r, &g, &b, &h);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void gaussian_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;

    if(SDL_LockSurface(surface) != 0)
        return;
    double sigma = sqrt((((x + 4) * (x + 4)) - 1) / 12);
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    double sum;
    gsl_matrix *filter = gaussian_filter(x, sigma, &sum);
    Uint32 *matrix = (Uint32 *)malloc(sizeof(Uint32) * x * x);
    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_around_x(surface, matrix, i, j, x);
            gaussian_average((size_t)x, sum, matrix, filter,
                    surface, &r, &g, &b, &a);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    copy_surface(surface2, surface);

    free(matrix);
    gsl_matrix_free(filter);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void fast_gaussian_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    double sigma = sqrt((((x + 4) * (x + 4)) - 1) / 12);
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    Uint32 *rect = (Uint32 *)malloc(sizeof(Uint32) * x);
    double sum;
    gsl_vector *filter = gaussian_filter_rect(x, sigma, &sum);
    printf("%f\n", sum);
    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_rect(surface, rect, i, j, x, 1);
            gaussian_average_rect(x, sum, rect, filter, surface, &r, &g, &b,
                    &a);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }
    copy_surface(surface2, surface);

    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_rect(surface, rect, i, j, x, 0);
            gaussian_average_rect(x, sum, rect, filter, surface, &r, &g, &b,
                    &a);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }

    copy_surface(surface2, surface);

    free(rect);
    gsl_vector_free(filter);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void motion_blur(SDL_Surface *surface, int x, double angle)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);

    Uint32 *rect = (Uint32 *)malloc(sizeof(Uint32) * (size_t)x);
    double cosa = cos(angle * (M_PI / 180));
    double sina = sin(angle * (M_PI / 180));

    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_rect_angle(surface, rect, i, j, x, cosa, sina);
            get_average_rect(surface, rect, &r, &g, &b, &a, x);
            set_pixel(surface2, r, g, b, a, i ,j);
        }
    }
    copy_surface(surface2, surface);

    free(rect);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

double len(int ax, int ay, int bx, int by)
{
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

void radial_blur(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);


    int midx = surface->w / 2;
    int midy = surface->h / 2;
    int ax = midx + ((x - 1) / 2);
    int ay = midy;
    int cx = midx - ((x - 1)/ 2);
    int cy = midy;

    double amid = len(0, 0, ax - midx, ay - midy);
    double cmid = len(0, 0, cx - midx, cy - midy);
    double max = len(0, 0, midx, midy);

    for(int j = 0; j < surface->h; j++)
    {
        for(int i = 0; i < surface->w; i++)
        {
            double angle;
            int strength;
            if(j - midy < 0)
            {
                double bmid = len(0, 0, i - midx, j - midy);
                double ab = len(ax - midx, ay - midy, i - midx, j - midy);
                angle = acos((bmid * bmid + amid * amid - ab * ab) /
                        (2 * amid * bmid));
                strength = (x * bmid) / max;
            }
            else if(j - midy == 0 && i - midx == 0)
            {
                Uint8 r, g, b, a;
                Uint32 pixel = get_pixel(surface, i, j);
                SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
                set_pixel(surface2, r, g, b, a, i, j);

                continue;
            }
            else
            {
                double bmid = len(0, 0, i - midx, j - midy);
                double cb = len(cx - midx, cy - midy, i - midx, j - midy);
                angle = acos((bmid * bmid + cmid * cmid - cb * cb) /
                        (2 * cmid * bmid));
                strength = (x * bmid) / max;
            }
            if(strength % 2 == 0)
            {
                strength += 1;
            }

            double cosa = cos(angle);
            double sina = sin(angle);

            Uint32 *rect = (Uint32 *)malloc(sizeof(Uint32) * (size_t)strength);

            Uint8 r, g, b, a;
            get_pixel_rect_angle(surface, rect, i, j, strength, cosa, sina);
            get_average_rect(surface, rect, &r, &g, &b, &a, strength);
            set_pixel(surface2, r, g, b, a, i ,j);
            free(rect);
        }
    }
    copy_surface(surface2, surface);

    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void surface_blur(SDL_Surface* surface, SDL_Surface* canny)
{
    SDL_Surface *blurred = create_surface(surface->w, surface->h);
    gsl_matrix* gaussian_filter = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(gaussian_filter, 0, 0, 0.0625);
    gsl_matrix_set(gaussian_filter, 2, 0, 0.0625);
    gsl_matrix_set(gaussian_filter, 0, 2, 0.0625);
    gsl_matrix_set(gaussian_filter, 2, 2, 0.0625);
    gsl_matrix_set(gaussian_filter, 0, 1, 0.125);
    gsl_matrix_set(gaussian_filter, 1, 0, 0.125);
    gsl_matrix_set(gaussian_filter, 1, 2, 0.125);
    gsl_matrix_set(gaussian_filter, 2, 1, 0.125);
    gsl_matrix_set(gaussian_filter, 1, 1, 0.25);

    for (int i = 0; i < surface->w; ++i)
    {
        for (int j = 0; j < surface->h; ++j)
        {
            Uint32 canny_pixel = get_pixel(canny, i, j);
            Uint8 canny_r, canny_g, canny_b;
            SDL_GetRGB(canny_pixel, canny->format, &canny_r, &canny_g,
                       &canny_b);
            if (canny_r == 0)
            {
                double nr, ng, nb;
                Uint8 a;
                nr = 0;
                ng = 0;
                nb = 0;
                a = 0;
                for (int k = -1; k < 2; ++k)
                {
                    for (int l = -1; l < 2; ++l)
                    {
                        Uint8 r, g, b;
                        double kernelvalue = gsl_matrix_get(gaussian_filter, 1
                        +k,1+l);

                        Uint32 pixel = get_pixel(surface,i + l,j
                        + k);
                        if (pixel == (Uint32) -1)
                        {
                            continue;
                        }
                        else
                        {
                            // Simple sum in function of neighbors with kernel
                            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                            double tmpr = kernelvalue * (double)r;
                            double tmpg = kernelvalue * (double)g;
                            double tmpb = kernelvalue * (double)b;
                            nr += tmpr;
                            ng += tmpg;
                            nb += tmpb;
                        }
                    }
                }
                nr = fabs(nr);
                ng = fabs(ng);
                nb = fabs(nb);
                set_pixel(blurred, (Uint8)nr, (Uint8)ng, (Uint8)nb, a, i, j);
            }
            else
            {
                Uint32 pixel = get_pixel(surface, i, j);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(blurred, r, g, b, a, i, j);
            }
        }
    }

    copy_surface(blurred, surface);

    SDL_FreeSurface(blurred);
    gsl_matrix_free(gaussian_filter);
}