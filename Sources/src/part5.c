#include "part5.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "font.h"
#include "sprite.h"
#include "interference.h"

#define PART5_LIFT_OPEN				53000
#define PART5_LIFT_OPEN_DURATION	1000
#define PART5_LIFT_CLOSE			58000
#define PART5_LIFT_CLOSE_DURATION	1000
#define PART5_LIFT_REOPEN			59000
#define PART5_LIFT_REOPEN_DURATION	3000
#define PART5_LIFT_RECLOSE			63000
#define PART5_LIFT_RECLOSE_DURATION	3000

extern T_Font *g_font;
static T_Sprite *g_ascenseur;
static SDL_Surface *g_interf;
static SDL_Surface *g_gfx_lift;

struct t_fade_timing part5_fade_timings[] =
{
	{ PART5_START_TIME, 500, 1}, 
	{ PART5_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

void part5_draw_lift(SDL_Surface *surf, SDL_Surface *dest, int lift_open)
{
	Uint32 *surf_pix, surf_w, color;
	int r, g, b, c;
	int x, y;
	if(lift_open > -1)
	{
		if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
		surf_pix = (Uint32 *)surf->pixels;
		surf_w = surf->pitch / surf->format->BytesPerPixel;
		for(y = 0; y < 240; y++)
		{
			for(x = 160-lift_open; x < 160+lift_open; x++)
			{
				/*color = surf_pix[x + y * surf_w];
				r = color & 0xFF;
				g = (color >> 8) & 0xFF;
				b = (color >> 16) & 0xFF;
				r >>= 3;
				g >>= 3;
				b >>= 3;
				color = (b<<16) | (g<<8) | r;
				
				surf_pix[x + y * surf_w] = 0xFF000000 | color;*/
				surf_pix[x + y * surf_w] = 0;
			}

			surf_pix[159 - lift_open + y * surf_w] = 0xFF808080;
			surf_pix[160 - lift_open + y * surf_w] = 0xFFFFFFFF;
			surf_pix[161 - lift_open + y * surf_w] = 0xFF808080;
			surf_pix[159 + lift_open + y * surf_w] = 0xFF808080;
			surf_pix[160 + lift_open + y * surf_w] = 0xFFFFFFFF;
			surf_pix[161 + lift_open + y * surf_w] = 0xFF808080;
		}
		if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
	}

	SDL_BlitSurface(surf, NULL, dest, NULL);
}

void part5_init(void)
{
	g_gfx_lift = utils_load_png_to_surface("data/graph_ascenseur.png");
	g_ascenseur = sprite_create_from_png("data/ascenceur_psy.png", 4, 100, 200, SPRITE_MODE_LOOP);
	g_interf = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	interference_init();
}

void part5_deinit(void)
{
	interference_deinit();
	SDL_FreeSurface(g_interf);
	SDL_FreeSurface(g_gfx_lift);
	sprite_destroy(g_ascenseur);
}

void part5_play(SDL_Surface *surf, unsigned current_time)
{
	int xouv;
	float time;
	Uint32 *surf_pix, surf_w;

	if(current_time < PART5_START_TIME) return;
	if(current_time > PART5_END_TIME) return;

	interference_draw(g_interf, current_time);
	
	SDL_FillRect(surf, NULL, 0xFF808080);
	if(current_time < PART5_LIFT_REOPEN) sprite_animate_and_blit(current_time, g_ascenseur, surf, 160-35, 80, 2);
	xouv = -1;
	if((current_time >= PART5_LIFT_OPEN))
	{
		time = current_time - PART5_LIFT_OPEN;
		time = time / (float)PART5_LIFT_OPEN_DURATION;
		time = time > 1.0f ? 1.0f : time;
		xouv = 70 * time;
	}
	if((current_time >= PART5_LIFT_CLOSE))
	{
		time = current_time - PART5_LIFT_CLOSE;
		time = 1.0f - (time / (float)PART5_LIFT_CLOSE_DURATION);
		time = time > 1.0f ? 1.0f : time;
		xouv = 70 * time;
	}
	if((current_time >= PART5_LIFT_REOPEN))
	{
		SDL_FillRect(surf, NULL, 0xFF000000);
		utils_blit_with_transparency(g_gfx_lift, surf, 0, 0);
		font_write("Code:",	160, 100,	  g_font, surf);
		font_write("flure", 180, 100 + 16, g_font, surf);
		time = current_time - PART5_LIFT_REOPEN;
		time = (time / (float)PART5_LIFT_REOPEN_DURATION);
		time = time > 1.0f ? 1.0f : time;
		xouv = 120 * time;
	}
	if((current_time >= PART5_LIFT_RECLOSE))
	{
		SDL_FillRect(surf, NULL, 0xFF000000);
		utils_blit_with_transparency(g_gfx_lift, surf, 0, 0);
		font_write("Code:",	160, 100,	  g_font, surf);
		font_write("flure", 180, 100 + 16, g_font, surf);
		time = current_time - PART5_LIFT_RECLOSE;
		time = 1.0f - (time / (float)PART5_LIFT_RECLOSE_DURATION);
		time = time > 1.0f ? 1.0f : time;
		xouv = 120 * time;
	}
	part5_draw_lift(g_interf, surf, xouv);

	effects_fade_manage(surf, current_time, part5_fade_timings);
}
