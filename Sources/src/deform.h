#ifndef DEFORM_H
#define DEFORM_H

#include "SDL.h"

void deform_draw_with_lut(unsigned current_time, SDL_Surface *src, SDL_Surface *dst, int *lut);
void deform_draw_with_lut_2(unsigned current_time, SDL_Surface *src, SDL_Surface *dst, int *lut1, int *lut2);

void deform_lut_identity(int *lut);

void deform_lut_a(int *lut);
void deform_lut_b(int *lut);
void deform_lut_c(int *lut);
void deform_lut_d(int *lut);

#endif // DEFORM_H