#ifndef INTERFERENCE_H
#define INTERFERENCE_H

#include "SDL.h"

void interference_init(void);
void interference_deinit(void);
void interference_draw(SDL_Surface *dst, unsigned current_time);

#endif // INTERFERENCE_H