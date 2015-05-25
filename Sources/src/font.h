#ifndef FONT_H
#define FONT_H

#include "SDL.h"

typedef struct t_font
{
	unsigned int char_width;
	unsigned int char_height;
	SDL_Surface *surf;
} T_Font;

T_Font *font_init(char *filename);
void font_deinit(T_Font *font);

void font_write(char *text, float x, float y, T_Font *font, SDL_Surface *dest);


#endif // FONT_H