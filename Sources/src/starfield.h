#ifndef STARFIELD_H
#define STARFIELD_H

#include "SDL.h"

void starfield_init(void);
void starfield_deinit(void);
void starfield_run(SDL_Surface *dest, int xc, int yc, float speed);

#endif // STARFIELD_H