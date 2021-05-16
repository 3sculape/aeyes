#include "basic.h"

void grayscale(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in grayscale");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 total = 0.21 * r + 0.72 * g + 0.07 * b;
            set_pixel(surface, total, total, total, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void binarization(SDL_Surface *surface, int threshold, int ra, int ga, int ba, int rb, int gb, int bb)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in binarization");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            if ((r + g + b) / 3 > threshold)
            {
                r = ra;
                g = ga;
                b = ba;
            }

            else
            {
                r = rb;
                g = gb;
                b = bb;
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void colorize(SDL_Surface *surface, int ra, int ga, int ba, int keep_luminance)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in binarization");
        return;
    }

    double hsv_input[3];
    rgb_to_hsl(ra, ga, ba, hsv_input);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            Uint8 rgb[3];
            double hsv2[3];
            rgb_to_hsl(r, g, b, hsv2);

            hsv2[0] = hsv_input[0];
            hsv2[1] = hsv_input[1];

            if (keep_luminance)
            {
                hsv2[2] = hsv2[2]/50 * hsv_input[2];
                if (hsv2[2] > 100)
                {
                    hsv2[2] = 100;
                }
            }

            else
            {
                hsv2[2] = hsv2[2]/100 * hsv_input[2];
            }

            hsl_to_rgb(hsv2[0], hsv2[1], hsv2[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void negative(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in negative");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void clipping(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in clipping");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            if (r == 255 && g == 255 && b == 255)
            {
                g = 0;
                b = 0;
            }

            else if (r == 0 && g == 0 && b == 0)
            {
                b = 255;
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void saturation(SDL_Surface *surface, double value)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in saturation");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            hsv[1] += value * (hsv[1] / 100);
            clamp(hsv[1], 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void exposure(SDL_Surface *surface, double value)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in exposure");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            hsv[2] += value * (hsv[2] / 100);
            clamp(hsv[2], 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void contrast(SDL_Surface *surface, double value)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in contrast");
        return;
    }

    double f = (259 * (value + 255)) / (255 * (259 - value));
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            r = clamp(f * (r - 128) + 128, 0, 255);
            g = clamp(f * (g - 128) + 128, 0, 255);
            b = clamp(f * (b - 128) + 128, 0, 255);

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

SDL_Surface* image_crop(SDL_Surface* original, size_t x, size_t y, size_t w,
        size_t h)
{
    SDL_Surface* cropped = create_surface(w, h);
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(original, x + i, y + j);
            SDL_GetRGBA(pixel, original -> format, &r, &g, &b, &a);
            set_pixel(cropped, r, g, b, a, i, j);
        }
    }
     return cropped;
}

SDL_Surface* resize(SDL_Surface* original, double factor)
{
    return rotozoomSurface(original, 0, factor, 1);
}

SDL_Surface* rotate(SDL_Surface* original, double angle)
{
    return rotozoomSurface(original, angle, 1, 1);
}

void mirror(SDL_Surface* surface, int xaxis)
{
    size_t w = surface->w;
    size_t h = surface->h;
    SDL_Surface* tmp = create_surface(surface->w, surface->h);
    if (xaxis)
    {
        for (size_t i = 0; i < w; i++)
        {
            for (size_t j = 0; j < h - 1; j++)
            {
                Uint32 pixel = get_pixel(surface, i, j);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(tmp, r, g, b, a, i, h - j - 1);
            }
        }

    }
    else
    {
        for (size_t i = 0; i < w - 1; i++)
        {
            for (size_t j = 0; j < h; j++)
            {
                Uint32 pixel = get_pixel(surface, i, j);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(tmp, r, g, b, a, w - i - 1, j);
            }
        }
    }
    copy_surface(tmp, surface);
    SDL_FreeSurface(tmp);
}

void mean(SDL_Surface* surface)
{
    SDL_Surface *mean = create_surface(surface->w, surface->h);
    long mean_r, mean_g, mean_b;
    mean_r = 0;
    mean_g = 0;
    mean_b = 0;
    long nb_pixels = surface->w * surface->h;
    for (int i = 0; i < surface->w; ++i)
    {
        for (int j = 0; j < surface->h; ++j)
        {
            Uint8 r, g, b;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            mean_r += r;
            mean_g += g;
            mean_b += b;
        }
    }
    mean_r /= nb_pixels;
    mean_g /= nb_pixels;
    mean_b /= nb_pixels;
    for (int i = 0; i < mean->w; ++i)
    {
        for (int j = 0; j < mean->h; ++j)
        {
            set_pixel(mean, mean_r, mean_g, mean_b, 1, i, j);
        }
    }
    copy_surface(mean, surface);
    SDL_FreeSurface(mean);
}

SDL_Surface *resize_fit_to_scale(SDL_Surface* original, double factor)
{
    SDL_Surface *resized = rotate(original, factor);
    if ((int) factor % 90 == 0)
        return resized;
    else
    {
        if (resized == NULL)
            errx(EXIT_FAILURE, "resize was NULL");
        /*double ratio = 2;
        SDL_Surface *tmp = resize(resized, ratio);
        SDL_Surface *fit = crop_from_center(tmp, tmp->w/(size_t)ratio,
                                            tmp->h/(size_t)ratio);
        */
        SDL_Surface *fit = zoom(resized, 2);
        return fit;
    }
}

SDL_Surface *crop_from_center(SDL_Surface* original, size_t w, size_t h)
{
    size_t center_x = (original->w / 2) - w/2;
    size_t center_y = (original->h / 2) - h/2;
    SDL_Surface *cropped = create_surface(w, h);
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(original, i + center_x, j + center_y);
            SDL_GetRGBA(pixel, original -> format, &r, &g, &b, &a);
            set_pixel(cropped, r, g, b, a, i, j);
        }
    }
    return cropped;
}

SDL_Surface* zoom(SDL_Surface* original, double factor)
{
    SDL_Surface *tmp = resize(original, factor);
    //printf("factor is %f\n", factor);
    saveJPG("rotozoom.JPG", tmp);
    if (factor < 1)
        return tmp;
    SDL_Surface *res = crop_from_center(tmp, original->w, original->h);
    SDL_FreeSurface(tmp);
    return res;
}

void symmetry(SDL_Surface* original, int y_axis, int direction)
{
    int limit_1, limit_2, offset_i, offset_j;
    if (y_axis)
    {
        limit_1 = original->w / 2;
        limit_2 = original->h;
        offset_i = original->w;
        offset_j = 0;
    }
    else
    {
        limit_1 = original->w;
        limit_2 = original->h / 2;
        offset_i = 0;
        offset_j = original->h;
    }

    if (!direction)
    {
        for (int i = 0; i < limit_1; i++)
        {
            for (int j = 0; j < limit_2; j++)
            {
                Uint8 r, g, b, a;
                Uint32 pixel = get_pixel(original, i, j);
                SDL_GetRGBA(pixel, original->format, &r, &g, &b, &a);
                set_pixel(original, r, g, b, a, abs(offset_i - i),abs(offset_j -
                                                                      j));
            }
        }
    }
    else
    {
        if (offset_i)
            limit_2 = 0;
        else
            limit_1 = 0;
        for (int i = original->w - 1; i > limit_1; --i)
        {
            for (int j = original->h - 1; j > limit_2; --j)
            {
                Uint8 r, g, b, a;
                Uint32 pixel = get_pixel(original, i, j);
                SDL_GetRGBA(pixel, original->format, &r, &g, &b, &a);
                set_pixel(original, r, g, b, a, abs(offset_i / 2 - (i -
                offset_i
                / 2)), abs(offset_j / 2 - (j -
                offset_j / 2)));
            }
        }
    }
}