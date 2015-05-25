#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include "SDL.h"

static Uint32 g_plasma_pal[256];

void plasma_init(void)
{
	int i;
	Uint32 r, g, b;
	for(i = 0; i < 256; i++)
	{
		r = 127 + 127 * sinf((255 - i) * _M_PI / 128.0f);
		g = 127 + 127 * cosf(i * _M_PI / 128.0f);
		b = 127 + 127 * sinf(i * _M_PI / 128.0f);
		g_plasma_pal[i] = 0xFF000000 + ((r&0xFF) << 16) + ((g&0xFF) << 8) + (b&0xFF);
	}
}

void plasma_pal_cycle(unsigned int current_time)
{
	Uint32 paltemp[256], i;
	static unsigned int last_time = 0;
	float time = (current_time - last_time) / 1000.0f;
	Uint32 offset = ((Uint32)(sinf(time) * 600)) % 256;

	for(i = 0; i < 256; i++)
	{
		paltemp[i] = g_plasma_pal[(i + offset)&0xFF];
	}
	memcpy(g_plasma_pal, paltemp, 256 * sizeof(Uint32));
	last_time = current_time;
}

void plasma_run(SDL_Surface *dest, unsigned int current_time)
{
	Uint32	*pixels, w;
	float	theta = current_time / 100.0f;
	int		x, y, x2, y2, xc, yc, yw;
	float	xx, yy, ctt, stt;
	int		index;
	float	ct = cosf(theta * 0.061655617f), st = sinf(theta * -0.36352262f);
	float	dist;

	xc = 160 + 50 * ct;
	yc = 120 + 50 * st;
	ctt = ct / 17.3f;
	stt = st / 31.9f;
	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	pixels = (Uint32 *)dest->pixels;
	w = dest->pitch / dest->format->BytesPerPixel;
	for(y = 0; y < dest->h; y++)
	{
		y2 = (yc - y) * (yc - y);
		yy = y * stt;
		yw = y * w;
		for(x = 0; x < dest->w; x++)
		{
			x2 = (xc - x) * (xc - x);
			xx = x * ctt;
			dist = (x2 + y2) / 200.0f;
			index = 127 + 64*(sinf(xx + yy) +  sinf(dist/17.0f+theta));
			pixels[x + yw] = g_plasma_pal[index];
		}
	}

	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
}