#ifndef EFFECTS_H
#define EFFECTS_H

#include <SDL2/SDL.h>
// apply the swirl effect on a surface
// factor value between 0 and 1 are the most pertinent
void swirl(SDL_Surface *surface, double factor);

// Apply the twist effect on a surface
// factor value between 0 and 10 are the most pertinent
void twist(SDL_Surface *surface, double factor);

#endif /* EFFECTS_H */
