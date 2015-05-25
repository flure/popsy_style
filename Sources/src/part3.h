#ifndef PART3_H
#define PART3_H

#include "SDL.h"

#define PART3_START_TIME	28911
#define PART3_END_TIME		43227

void part3_init(void);
void part3_deinit(void);
void part3_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H