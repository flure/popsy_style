#ifndef PLASMA_H
#define PLASMA_H

#include "SDL.h"

void plasma_init(void);
void plasma_run(SDL_Surface *dest, unsigned int current_time);
void plasma_pal_cycle(unsigned int current_time);

#endif // PLASMA_H