#ifndef TUNNEL_H
#define TUNNEL_H

#include "SDL.h"

void tunnel_init(void);
void tunnel_deinit(void);
void tunnel_run(SDL_Surface *dest, SDL_Surface *tex, float avance, float tourne, float xc, float yc);

#endif // TUNNEL_H