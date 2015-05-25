#ifndef PART4_H
#define PART4_H

#include "SDL.h"

#define PART4_START_TIME	43227
#define PART4_END_TIME		50058

void part4_init(void);
void part4_deinit(void);
void part4_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H