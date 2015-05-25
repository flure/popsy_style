#ifndef PART1_H
#define PART1_H

#include "SDL.h"

#define PART1_START_TIME	0
#define PART1_END_TIME		14596

void part1_init(void);
void part1_deinit(void);
void part1_play(SDL_Surface *surf, unsigned current_time);


#endif // PART1_H