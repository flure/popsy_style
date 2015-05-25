#include "tunnel.h"

#include <math.h>
#include <stdlib.h>
#include "SDL.h"
#include "utils.h"

#include <stdio.h>

static int *dist_lut;
static int *angles_lut;
#define TUNNEL_RADIUS	200
#define EYE_DIST		50

#define TUNNEL_WIDTH	640
#define TUNNEL_HEIGHT	480
#define TUNNEL_TEX_SIZE	512

void tunnel_init(void)
{
	int x, y, offset;
	double fx, fy, d, theta;

	dist_lut = (int *)malloc(TUNNEL_WIDTH * TUNNEL_HEIGHT * sizeof(int));
	angles_lut = (int *)malloc(TUNNEL_WIDTH * TUNNEL_HEIGHT * sizeof(int));

	offset = 0;
	for(y = 0; y < TUNNEL_HEIGHT; y++)
	{
		fy = y - TUNNEL_HEIGHT/2.0;
		for(x = 0; x < TUNNEL_WIDTH; x++)
		{
			fx = x - TUNNEL_WIDTH/2.0;
			fx = abs(fx) <= 0.1 ? 0.1 : fx;

			theta = atan2(fy, fx);
			d = cos(theta) * TUNNEL_RADIUS * EYE_DIST / fx;
			dist_lut[offset] = (int)d;
			angles_lut[offset] = ((int)(theta * (TUNNEL_TEX_SIZE / 2) / _M_PI));

			offset++;
		}
	}
}

void tunnel_deinit(void)
{
	free(dist_lut);
	free(angles_lut);
}

void tunnel_run(SDL_Surface *dest, SDL_Surface *tex, float avance, float tourne, float xc, float yc)
{
	Uint32 *dst_pix, *tex_pix;
	Uint32 dst_w, tex_w, c, d, a, z;
	int x, y, offset;
	float center_dist;
	int lum;
	int centre_x, centre_y, av, to;

	centre_x = (int)(xc+0.5f);
	centre_y = (int)(yc+0.5f);
	av = (int)(avance+0.5f);
	to = (int)(tourne+0.5f);
	
	if(SDL_MUSTLOCK(tex)) SDL_LockSurface(tex);
	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	dst_pix = (Uint32 *)dest->pixels;
	dst_w = dest->pitch / dest->format->BytesPerPixel;
	tex_pix = (Uint32 *)tex->pixels;
	tex_w = tex->pitch / tex->format->BytesPerPixel;
	offset = 0;
	for(y = 0; y < 240; y++)
	{
		offset = (y + centre_y) * TUNNEL_WIDTH;
		for(x = 0; x < 320; x++)
		{
			//center_dist = sqrtf((y - centre_y) * (y - centre_y) + (x - centre_x) * (x - centre_x));
			//center_dist *= (256 / 64);
			//lum = (center_dist > 255 ? 0: (int)center_dist);
			// TODO : lumière au fond du tunnel (ou pas)

			z = dist_lut[offset + centre_x];
			d = (z + av) % TUNNEL_TEX_SIZE;
			a = angles_lut[offset + centre_x];
			a = (a + to) % TUNNEL_TEX_SIZE;

			c = tex_pix[a + d * tex_w] | 0xFF000000;
			dst_pix[x + y * dst_w] = c;

			offset++;
		}
	}
	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
	if(SDL_MUSTLOCK(tex)) SDL_UnlockSurface(tex);
}
