#include "starfield.h"

#include <math.h>
#include <stdlib.h>
#include "SDL.h"
#include "utils.h"

#include <stdio.h>

#define STARS_COUNT 1000
#define SPACE_SCALE	75
#define Z_FAR		1.0f
#define Z_NEAR		0.001f

static SDL_Surface *g_starfield_surf = NULL;

typedef struct t_float3
{
	float x, y, z;
} T_Float3;

typedef struct t_star
{
	T_Float3 xyz;
	T_Float3 rgb;
} T_Star;

static T_Star *g_stars;

void _project_point(T_Float3 space, T_Float3 *screen, float dist_foc)
{
	float Z = space.z / Z_FAR;
	screen->x = space.x * dist_foc / Z;
	screen->y = space.y * dist_foc / Z;
}

void _reinit_star(int star_index)
{
	g_stars[star_index].xyz.x = ((rand()%1000)/500.0f) - 1.0f;
	g_stars[star_index].xyz.y = ((rand()%1000)/500.0f) - 1.0f;
	g_stars[star_index].xyz.z = Z_FAR;
		
	g_stars[star_index].rgb.x = 0.5f + ((rand()%500)) / 1000.0f;
	g_stars[star_index].rgb.y = 0.5f + ((rand()%500)) / 1000.0f;
	g_stars[star_index].rgb.z = 0.5f + ((rand()%500)) / 1000.0f;
}

void starfield_init(void)
{
	unsigned int i;
	g_starfield_surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	g_stars = (T_Star *)malloc(STARS_COUNT * sizeof(T_Star));
	for(i = 0; i < STARS_COUNT; i++)
	{
		g_stars[i].xyz.x = ((rand()%1000)/500.0f) - 1.0f;
		g_stars[i].xyz.y = ((rand()%1000)/500.0f) - 1.0f;
		g_stars[i].xyz.z = ((rand()%1000)/1000.0f) * Z_FAR;

		g_stars[i].rgb.x = 0.5f + ((rand()%500)) / 1000.0f;
		g_stars[i].rgb.y = 0.5f + ((rand()%500)) / 1000.0f;
		g_stars[i].rgb.z = 0.5f + ((rand()%500)) / 1000.0f;
	}
}

void starfield_deinit(void)
{
	SDL_FreeSurface(g_starfield_surf);
	free(g_stars);
}

void starfield_run(SDL_Surface *dest, int xc, int yc, float speed)
{
	unsigned int i;
	int x, y;
	float X, Y, Z;
	Uint32 *dest_pixels = NULL, color;
	Uint32 r, g, b;
	T_Float3 p_world, p_screen;

	//SDL_FillRect(dest, NULL, 0);
	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	dest_pixels = (unsigned int *)dest->pixels;
	for(i = 0; i < STARS_COUNT; i++)
	{
		g_stars[i].xyz.z += speed;

		if(g_stars[i].xyz.z <= Z_NEAR) _reinit_star(i);
		//if(fabs(g_stars[i].xyz.z) >= 1.0f) _reinit_star(i);//g_stars[i].xyz.z = -g_stars[i].xyz.z;
		//if(g_stars[i].xyz.z <= 0.0001f) continue;

		Z = (Z_FAR - g_stars[i].xyz.z) / (Z_FAR - Z_NEAR);
		r = (unsigned int)(255 * g_stars[i].rgb.x * Z);
		g = (unsigned int)(255 * g_stars[i].rgb.y * Z);
		b = (unsigned int)(255 * g_stars[i].rgb.z * Z);
		/*r = (Uint32)(255 * g_stars[i].rgb.x);
		g = (Uint32)(255 * g_stars[i].rgb.y);
		b = (Uint32)(255 * g_stars[i].rgb.z);*/
		color = 0xFF000000 + (b&0xFF) + ((g&0xFF)<<8) + ((r&0xFF)<<16);
		//color = 0xFF000000 + (r&0xFF) + ((r&0xFF)<<8) + ((r&0xFF)<<16);

		p_world.x = g_stars[i].xyz.x;
		p_world.y = g_stars[i].xyz.y;
		p_world.z = g_stars[i].xyz.z;
		_project_point(p_world, &p_screen, 1.0f);
		
		x = (int)(xc + SPACE_SCALE * p_screen.x);
		y = (int)(yc + SPACE_SCALE * p_screen.y);

		if(x >= 320) continue;
		if(y >= 240) continue;
		if(x < 0) continue;
		if(y < 0) continue;
		
		if((x > 0) && (y > 0)) dest_pixels[x-1 + (y-1)*320] = color;
		if((x < 319) && (y > 0)) dest_pixels[x+1 + (y-1)*320] = color;
		if((x > 0) && (y < 239)) dest_pixels[x-1 + (y+1)*320] = color;
		if((x < 319) && (y <239)) dest_pixels[x+1 + (y+1)*320] = color;
		if(x > 0)	dest_pixels[x-1 + y*320] = color;
		if(x < 319) dest_pixels[x+1 + y*320] = color;
		dest_pixels[x + y*320] = color;
		if(y > 0)	dest_pixels[x + (y-1)*320] = color;
		if(y < 239)	dest_pixels[x + (y+1)*320] = color;
	}
	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
}
