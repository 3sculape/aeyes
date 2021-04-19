#ifndef VIGNETTE_H
#define VIGNETTE_H

/*
    Apply the amount <amount> of vignette effect to the
    provided surface.
    <surface>: the surface to be processed
    <amount>: the amount of vignette to apply [0, 100]
*/
void vignette(SDL_Surface *surface, double amount);

#endif