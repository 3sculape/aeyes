#include "utility.h"

void print_matrix(gsl_matrix *m)
{
    for (size_t i = 0; i < m -> size1; i++)
    {
        for (size_t j = 0; j < m -> size2; j++)
            printf("%f ", gsl_matrix_get(m, i, j));
        printf("\n");
    }
}

void print_vector(gsl_vector *v)
{
    for (size_t i = 0; i < v -> size; i++)
        printf("%f ", gsl_vector_get(v, i));
    printf("\n");
}

double clamp(double x, double min, double max)
{
    if(x < min)
        return min;
    else if(x > max)
        return max;
    else
        return x;
}

double distance(int ax, int ay, int bx, int by)
{
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

double lerp(double a, double b, double w)
{
    return w * a + (1 - w) * b;
}

double blerp(double a, double b, double c, double d, double tx, double ty)
{
	double x = lerp(lerp(a, b, tx), lerp(c, d, tx), ty);
	return x;
}

double fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

int otsu_threshold(SDL_Surface* org)
{
    int histo[256];
    int total_p = org->w * org->h;
    int sum = 0;
    int maxratio = 0;
    int var_max = 0;
    int sumB = 0;
    int q1 = 0;
    int q2;
    int mu1;
    int mu2;
    for (int i = 0; i < 256; ++i)
    {
        histo[i] = 0;
    }
    for (int i = 0; i < org->w; ++i)
    {
        for (int j = 0; j < org->h; ++j)
        {
            Uint8 r, g, b;
            Uint32 pix = get_pixel(org, i, j);
            SDL_GetRGB(pix, org->format, &r, &g, &b);
            histo[r] += 1;
        }
    }
    for (int i = 0; i < 256; i++)
    {
        sum += i * histo[i];
    }

    for (int t = 0; t < 256; t++)
    {
        q1 += histo[t];
        if (!q1)
            continue;
        q2 = total_p - q1;
        if (!q2)
            break;
        sumB += t * histo[t];
        mu1 = sumB / q1;
        mu2 = (sum - sumB) / q2;
        int diff = mu1 - mu2;

        int sigma = q1 * q2 * diff*diff;

        if (sigma > var_max)
        {
            maxratio = t;
            var_max = sigma;
        }
    }
    return maxratio;
}

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