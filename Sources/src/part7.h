#ifndef PART7_H
#define PART7_H

#include "SDL.h"

#define PART7_START_TIME	80000
#define PART7_END_TIME		101000

void part7_init(void);
void part7_deinit(void);
void part7_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H