#include "font.h"

#include "SDL.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


T_Font *font_init(char *filename)
{
	T_Font *result = (T_Font *)malloc(sizeof(T_Font));
	result->surf = utils_load_png_to_surface(filename);
	result->char_width = result->surf->w / 16;
	result->char_height = result->surf->h / 16;
	SDL_SetColorKey(result->surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0xFF00FF);
	return result;
}

void font_deinit(T_Font *font)
{
	SDL_FreeSurface(font->surf);
	free(font);
}

void font_write(char *text, float x, float y, T_Font *font, SDL_Surface *dest)
{
	unsigned int i, row, col;
	SDL_Rect src, dst;
	int X, Y;
	X = x + 0.5f;
	Y = y + 0.5f;

	for(i = 0; i < strlen(text); i++)
	{
		row = text[i] / 16;
		col = text[i] % 16;
		src.x = col * font->char_width;
		src.y = row * font->char_height;
		src.w = font->char_width;
		src.h = font->char_height;

		dst.x = X + i * font->char_width;
		dst.y = Y;

		if((dst.x < -src.w) || (dst.x >= 320 + src.w)) continue;
		if((dst.y < -src.h) || (dst.y >= 240 + src.h)) continue;

		SDL_BlitSurface(font->surf, &src, dest, &dst);
	}
}