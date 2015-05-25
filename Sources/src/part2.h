#ifndef PART2_H
#define PART2_H

#include "SDL.h"

#define PART2_START_TIME	14596
#define PART2_END_TIME		28911

void part2_init(void);
void part2_deinit(void);
void part2_play(SDL_Surface *surf, unsigned current_time);


#endif // PART2_H