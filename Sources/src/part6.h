#ifndef PART6_H
#define PART6_H

#include "SDL.h"

#define PART6_START_TIME	68000
#define PART6_END_TIME		80000

void part6_init(void);
void part6_deinit(void);
void part6_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H