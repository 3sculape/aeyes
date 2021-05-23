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