#include "noise.h"

unsigned int hash(unsigned int x, unsigned int y)
{
    x = (x << 16) | (y & 0xffff);

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;

    return x;
}

double grad(size_t corner_x, size_t corner_y, double dist_x, double dist_y)
{
        double pi_4 = cos(3.14 / 4.0);

        switch (hash(corner_x, corner_y) & 8)
        {
            case 0:
                return +dist_x;
            case 1:
                return +dist_y;
            case 2:
                return -dist_x;
            case 3:
                return -dist_y;
            case 4:
                return dist_x * pi_4 + dist_y * pi_4;
            case 5:
                return -dist_x * pi_4 + dist_y * pi_4;
            case 6:
                return -dist_x * pi_4 - dist_y * pi_4;
            case 7:
                return dist_x * pi_4 - dist_y * pi_4;
            default:
                warnx("perlin: grad function failed");
                return 0.0;
        }
}


double perlin(double x, double y)
{
    size_t X = (size_t)floor(x);
    size_t Y = (size_t)floor(y);

    x -= floor(x);
    y -= floor(y);

    double grad00 = grad(X, Y, x, y);
    double grad01 = grad(X, Y + 1, x, y - 1.0);
    double grad10 = grad(X + 1, Y, x - 1.0, y);
    double grad11 = grad(X + 1, Y + 1, x - 1.0, y - 1.0);

    double u = fade(x);
    double v = fade(y);

    return lerp(lerp(grad00, grad10, u), lerp(grad01, grad11, u), v);
}