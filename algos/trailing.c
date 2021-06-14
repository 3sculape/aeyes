#include "trailing.h"

void global_trailing_v(SDL_Surface *surface, int start_pixel, int inverse) {
  int image_h = surface->h;
  int image_w = surface->w;
  Uint8 r, g, b, a;
  Uint32 pixel;
  int kernel_size;

  if (inverse == 0) {
    kernel_size = image_w - start_pixel;

    for (int j = 0; j < image_h; j++) {
      pixel = get_pixel(surface, start_pixel, j);
      SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

      for (int i = 1; i < kernel_size; i++) {
        set_pixel(surface, r, g, b, a, start_pixel + i, j);
      }
    }
  }

  if (inverse == 1) {
    kernel_size = start_pixel;

    for (int j = 0; j < image_h; j++) {
      pixel = get_pixel(surface, start_pixel, j);
      SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

      for (int i = start_pixel - 1; i >= 0; i--) {
        set_pixel(surface, r, g, b, a, i, j);
      }
    }
  }
}

void global_trailing_h(SDL_Surface *surface, int start_pixel, int inverse) {
  int image_h = surface->h;
  int image_w = surface->w;
  Uint8 r, g, b, a;
  Uint32 pixel;
  int kernel_size;

  if (inverse == 0) {
    kernel_size = image_h - start_pixel;

    for (int i = 0; i < image_w; i++) {
      pixel = get_pixel(surface, i, start_pixel);
      SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

      for (int j = 1; j < kernel_size; j++) {
        set_pixel(surface, r, g, b, a, i, j + start_pixel);
      }
    }
  }

  if (inverse == 1) {
    kernel_size = start_pixel;

    for (int i = 0; i < image_w; i++) {
      pixel = get_pixel(surface, i, start_pixel);
      SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

      for (int j = start_pixel - 1; j >= 0; j--) {
        set_pixel(surface, r, g, b, a, i, j);
      }
    }
  }
}

void edge_trailing_soufflerie(SDL_Surface *surface, SDL_Surface *edge_map,
                              int kernel_size, int inverse) {
  int image_h = surface->h;
  int image_w = surface->w;
  Uint8 source_r, source_g, source_b, source_a;
  Uint32 source_pixel;
  Uint8 edge_r, edge_g, edge_b, edge_a;
  Uint32 edge_pixel;

  if (inverse == 0) {
    for (int j = 0; j < image_h; j++) {
      for (int i = 0; i < image_w; i++) {
        edge_pixel = get_pixel(edge_map, i, j);
        SDL_GetRGBA(edge_pixel, edge_map->format, &edge_r, &edge_g, &edge_b,
                    &edge_a);
        if (edge_r == 255) {
          source_pixel = get_pixel(surface, i, j);
          SDL_GetRGBA(source_pixel, surface->format, &source_r, &source_g,
                      &source_b, &source_a);
          for (int x = 0; x < kernel_size; x++) {
            set_pixel(surface, source_r, source_g, source_b, source_a, i + x,
                      j);
          }
        }
      }
    }
  }

  else {
    for (int j = 0; j < image_h; j++) {
      for (int i = 0; i < image_w; i++) {
        edge_pixel = get_pixel(edge_map, i, j);
        SDL_GetRGBA(edge_pixel, edge_map->format, &edge_r, &edge_g, &edge_b,
                    &edge_a);
        if (edge_r == 255) {
          source_pixel = get_pixel(surface, i, j);
          SDL_GetRGBA(source_pixel, surface->format, &source_r, &source_g,
                      &source_b, &source_a);
          for (int x = 0; x < kernel_size; x++) {
            set_pixel(surface, source_r, source_g, source_b, source_a, i - x,
                      j);
          }
        }
      }
    }
  }
}

void edge_trailing_zigzag(SDL_Surface *surface, SDL_Surface *edge_map,
                          int kernel_size) {
  int image_h = surface->h;
  int image_w = surface->w;
  Uint8 source_r, source_g, source_b, source_a;
  Uint32 source_pixel;
  Uint8 edge_r, edge_g, edge_b, edge_a;
  Uint32 edge_pixel;
  int random_kernel = kernel_size;

  srand(time(NULL));

  for (int j = 0; j < image_h; j++) {
    for (int i = 0; i < image_w; i++) {
      random_kernel = rand() % kernel_size;
      edge_pixel = get_pixel(edge_map, i, j);
      SDL_GetRGBA(edge_pixel, edge_map->format, &edge_r, &edge_g, &edge_b,
                  &edge_a);
      if (edge_r == 255) {
        if (i % 2 == 0) {
          source_pixel = get_pixel(surface, i, j);
          SDL_GetRGBA(source_pixel, surface->format, &source_r, &source_g,
                      &source_b, &source_a);
          for (int x = 0; x < random_kernel; x++) {
            set_pixel(surface, source_r, source_g, source_b, source_a, i + x,
                      j);
          }
        }

        else {
          source_pixel = get_pixel(surface, i, j);
          SDL_GetRGBA(source_pixel, surface->format, &source_r, &source_g,
                      &source_b, &source_a);
          for (int x = 0; x < random_kernel; x++) {
            set_pixel(surface, source_r, source_g, source_b, source_a, i - x,
                      j);
          }
        }
      }
    }
  }
}
