#ifndef PART5_H
#define PART5_H

#include "SDL.h"

#define PART5_START_TIME	50058
#define PART5_END_TIME		68000

void part5_init(void);
void part5_deinit(void);
void part5_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H