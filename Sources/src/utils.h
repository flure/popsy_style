#ifndef UTILS_H
#define UTILS_H

#include "SDL.h"

#define _M_PI 3.141592653589f

SDL_Surface* utils_load_png_to_surface(char *filename);
void utils_blit_with_transparency(SDL_Surface *src, SDL_Surface *dest, float x, float y);

#endif // UTILS_H