#include "seam_carving.h"
#include <gsl/gsl_matrix.h>
#include <math.h>

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

double min_two_double(double a, double b)
{
    if(a > b)
        return b;
    return a;
}



double min_three_double(double a, double b, double c)
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


double min_index_three_double(double a, double b, double c)
{
    if (a > b)
    {
        if (c > a)
            return 0;
        if (c < b)
            return 1;
        return 0;
    }

    else if (a == b)
    {
        if (c < b)
            return 1;
        else
            return 0;
    }
    

    else
    {
        if (c > b)
            return -1;
        if (c > a)
            return -1;
        return 1;
    }
}


int min_two_index_left(int a, int b)
{
    if(a > b)
        return 1;
    return 0;
}

int min_two_index_right(int a, int b)
{
    if(a < b)
        return -1;
    return 0;
}

SDL_Surface* power_map_new(SDL_Surface *surface, SDL_Surface *edge_map, int nb_to_remove)
{
    int image_h = surface->h;
    int image_w = surface->w;

    SDL_Surface* power_map = create_surface(image_w, image_h);

    SDL_Surface* surface_update = create_surface(image_w, image_h);
    copy_surface(surface, surface_update);

    gsl_matrix* power_map_matrix = gsl_matrix_calloc(image_w, image_h);

    Uint8 sr, sg, sb, sa;
    Uint32 s_pixel;

    Uint8 r1, g1, b1, a1;
    Uint32 pixel1;
    Uint8 r2, g2, b2, a2;
    Uint32 pixel2;
    Uint8 r3, g3, b3, a3;
    Uint32 pixel3;

    Uint8 min = 0;

    double p_current;
    double p_left;
    double p_mid;
    double p_right;
    double p_min;

    for (int j = image_h-1; j >= 0; j--)
    {
        for (int i = 1; i < image_w-1; i++)
        {
            s_pixel = get_pixel(edge_map, i, j);
            SDL_GetRGBA(s_pixel, edge_map->format, &sr, &sg, &sb, &sa);

            p_current = (double) sr;

            if (j == image_h-1)  //first row of power init
            {
                gsl_matrix_set(power_map_matrix, i, j, p_current);
            }

            else
            {
                if (i == 1)
                {
                    p_mid = gsl_matrix_get(power_map_matrix, i, j+1);
                    p_right = gsl_matrix_get(power_map_matrix, i+1, j+1);
                    p_min = (min_two_double(p_mid, p_right));
                    gsl_matrix_set(power_map_matrix, i , j, p_min+p_current);
                }

                else if (i == image_w-2)
                {
                    p_left = gsl_matrix_get(power_map_matrix, i-1, j+1);
                    p_mid = gsl_matrix_get(power_map_matrix, i, j+1);
                    p_min = (min_two_double(p_left, p_mid));
                    gsl_matrix_set(power_map_matrix, i , j, p_min+p_current);
                }
                
                
                else
                {
                    p_left = gsl_matrix_get(power_map_matrix, i-1, j+1);
                    p_mid = gsl_matrix_get(power_map_matrix, i, j+1);
                    p_right = gsl_matrix_get(power_map_matrix, i+1, j+1);

                    p_min = (min_three_double(p_left, p_mid, p_right));

                    gsl_matrix_set(power_map_matrix, i , j, p_min+p_current);
                }
            }
        }
        
    }

    

    double general_max = gsl_matrix_max(power_map_matrix);
    double divisor_factor = 255/general_max;

    int replace_pixel = 0;

    for (int j = image_h-1; j >= 0; j--)
    {
        for (int i = 0; i < image_w; i++)
        {
            replace_pixel = (int)(gsl_matrix_get(power_map_matrix, i, j)* divisor_factor);
            set_pixel(power_map, replace_pixel, replace_pixel, replace_pixel, sa, i, j);
        }
    }


    int* to_remove = malloc((image_h)*(sizeof(int)));


  
    for (size_t x = 0; x < nb_to_remove; x++)
    {

        double min_of_row;
        min_of_row = 1;
        for (size_t i = 2; i < image_w-1; i++)
        {
            if (gsl_matrix_get(power_map_matrix, i, 0) < gsl_matrix_get(power_map_matrix, min_of_row, 0))
            {
                min_of_row = i;
            }
            
        }
        


        double current_index = min_of_row;
        double left;
        double mid;
        double right;

        double test1;
        double test2;

        

        for (int j = 0; j < image_h; j++)
        {
            to_remove[j] = current_index;

            if (j != image_h-1)
            {
                if (current_index == 1)
                {
                    mid = gsl_matrix_get(power_map_matrix, (size_t)(current_index), (size_t)(j+1));
                    right = gsl_matrix_get(power_map_matrix, (size_t)(current_index+1), (size_t)(j+1));
                    current_index += min_two_index_left(mid, right);
                }

                else if (current_index == image_w-2)
                {
                    left = gsl_matrix_get(power_map_matrix, (size_t)(current_index-1), (size_t)(j+1));
                    mid = gsl_matrix_get(power_map_matrix, (size_t)(current_index), (size_t)(j+1));
                    current_index += min_two_index_right(left, mid);
                }
                
                else
                {
                    test1 = current_index-1;
                    test2 = j+1;
                    left = gsl_matrix_get(power_map_matrix, (size_t)(current_index-1), (size_t)(j+1));
                    mid = gsl_matrix_get(power_map_matrix, (size_t)(current_index), (size_t)(j+1));
                    right = gsl_matrix_get(power_map_matrix, (size_t)(current_index+1), (size_t)(j+1));
                    current_index += min_index_three_double(left, mid, right);
                }
            }
        }

        Uint8 pr, pg, pb, pa;
        Uint32 p_pixel;

        SDL_Surface* update_power = create_surface(image_w-1, image_h);
        SDL_Surface* update_og = create_surface(image_w-1, image_h);
        gsl_matrix* power_map_update_matrix = gsl_matrix_calloc(image_w-1, image_h);
        

        for (size_t j = 0; j < image_h; j++)
        {
            int index = 0;

            for (size_t i = 0; i < image_w; i++)
            {
                if (to_remove[j] != i)
                {
                    p_pixel = get_pixel(power_map, i, j);
                    SDL_GetRGBA(p_pixel, power_map->format, &pr, &pg, &pb, &pa);

                    set_pixel(update_power, pr, pg, pb, pa, index, j);


                    p_pixel = get_pixel(surface_update, i, j);
                    SDL_GetRGBA(p_pixel, surface_update->format, &pr, &pg, &pb, &pa);

                    set_pixel(update_og, pr, pg, pb, pa, index, j);

                    gsl_matrix_set(power_map_update_matrix, index, j, gsl_matrix_get(power_map_matrix, i, j));

                    index += 1;
                }
                
            }
            
        }

        image_w -= 1;

        SDL_FreeSurface(power_map);
        SDL_FreeSurface(surface_update);
        gsl_matrix_free(power_map_matrix);

        power_map_matrix = power_map_update_matrix;
        power_map = update_power;
        surface_update = update_og;
    }

    free(to_remove);
    savePNG("./power_map.png", power_map);
    SDL_FreeSurface(power_map);
    gsl_matrix_free(power_map_matrix);

    return surface_update;
    
}


SDL_Surface* black_white(SDL_Surface *surface)
{
    int h = surface->h;
    int w = surface->w;

    SDL_Surface* res = create_surface(w, h);

    Uint8 pr, pg, pb, pa;
    Uint32 p_pixel;

    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        {
            p_pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(p_pixel, surface->format, &pr, &pg, &pb, &pa);
            int sum = (pr+pg+pb)/3;
            set_pixel(res, sum, sum, sum, pa, i, j);
        }
    }

    return res;
}


SDL_Surface* seam_hypot(SDL_Surface *surface)
{
    int h = surface->h;
    int w = surface->w;

    SDL_Surface* hypot = create_surface(w, h);

    SDL_Surface* b_w = black_white(surface);

    double sobel_x[] = {-0.125, 0, 0.125, -0.25, 0, 0.25, -0.125, 0, 0.125};
    double sobel_y[] = {-0.125, -0.25, -0.125, 0, 0, 0, 0.125, 0.25, 0.125};

    Uint8 pr, pg, pb, pa;
    Uint32 p_pixel;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            double three_sum_x = 0;
            double three_sum_y = 0;

            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    if (j-1+y < 0 || j+y > h)
                    {
                        if (i-1+x < 0 || i+x > w)
                        {
                            // corners
                            p_pixel = get_pixel(b_w, i, j);
                        }

                        else
                        {
                            // borders
                            p_pixel = get_pixel(b_w, i-1+x, j);
                        }
                        
                    }

                    else
                    {
                        if (i-1+x < 0 || i+x > w)
                        {
                            // borders
                            p_pixel = get_pixel(b_w, i, j-1+y);
                        }

                        else
                        {
                            // inner safe space
                            p_pixel = get_pixel(b_w, i-1+x, j-1+y);
                        }
                    }

                    SDL_GetRGBA(p_pixel, surface->format, &pr, &pg, &pb, &pa);
                    three_sum_x += pr * sobel_x[y*3 + x];
                    three_sum_y += pr * sobel_y[y*3 + x];

                }
                
            }

            double grad_x = abs(three_sum_x);
            double grad_y = abs(three_sum_y);

            Uint8 grad_hyp = sqrt((grad_x*grad_x)+(grad_y*grad_y));

            set_pixel(hypot, grad_hyp, grad_hyp, grad_hyp, pa, i, j);
            
        }
        
    }

    savePNG("./new_hypot.png", hypot);
    SDL_FreeSurface(b_w);

    return hypot;
    
}


/* SDL_Surface* power_map_ultimate(SDL_Surface *surface, int nb_to_remove)
{

} */