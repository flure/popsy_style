#include "part7.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "font.h"
#include "sprite.h"
#include "tunnel.h"
#include "plasma.h"
#include "interference.h"
#include "starfield.h"
#include "effects.h"

extern T_Font *g_font;
static SDL_Surface *g_tunnel_tex;
static T_Sprite *g_butt;

void part7_do_plasma(unsigned current_time, SDL_Surface *surf)
{
	plasma_run(surf, current_time);
	plasma_pal_cycle(current_time);
}

void part7_do_tunnel(unsigned current_time, SDL_Surface *surf)
{
	int x_tunnel = 50.0 + 40.0 * sin(current_time / 850.0f);
	int y_tunnel = 120.0 + 40.0 * cos(current_time / 650.0f);
	tunnel_run(surf, g_tunnel_tex, current_time / 5.0, current_time / 4.0, 
				x_tunnel, y_tunnel);
}

void part7_do_interf(unsigned current_time, SDL_Surface *surf)
{
	interference_draw(surf, current_time);
}

void part7_do_starfield(unsigned current_time, SDL_Surface *surf)
{
	starfield_run(surf, 160 + 40 * sin(current_time / 1000.0f), 120 + 40 * sin(current_time / 1000.0f), -0.01f);
	effects_blur(surf);
}

struct t_fade_timing part7_fade_timings[] =
{
	{ PART7_START_TIME, 500, 1}, 
	{ PART7_END_TIME-1000, 1000, -1}, 
	{ 0, 0}
};

void part7_init(void)
{
	plasma_init();
	g_tunnel_tex = utils_load_png_to_surface("data/tunnel.png");
	tunnel_init(); // XXX A VERIFIER
}

void part7_deinit(void)
{
	//tunnel_deinit(); // XXX A VERIFIER
	SDL_FreeSurface(g_tunnel_tex);
}

void part7_play(SDL_Surface *surf, unsigned current_time)
{
	unsigned local_time, choose;
	float t_gfx;
	int x_gfx, x_tunnel, y_tunnel, x_car, y_car;
	float x_credit, y_credit;
	static int current_effect = 0;
	static unsigned last_change_time = PART7_START_TIME;
	if(current_time < PART7_START_TIME) return;
	if(current_time > PART7_END_TIME) return;

	local_time = current_time - last_change_time;
	if(local_time > 1000)
	{
		current_effect++;
		last_change_time = current_time;
	}

	switch(current_effect % 4)
	{
	case 0: part7_do_interf(current_time, surf); break;
	case 1: part7_do_plasma(current_time, surf); break;
	case 2: part7_do_starfield(current_time, surf); break;
	case 3: part7_do_tunnel(current_time, surf); break;
	}

	/*
	x_tunnel = 50.0 + 40.0 * sin(current_time / 850.0f);
	y_tunnel = 120.0 + 40.0 * cos(current_time / 650.0f);
	tunnel_run(surf, g_tunnel_tex, current_time / 5.0, current_time / 4.0, 
				x_tunnel, y_tunnel);
	x_car = 70 +160 - x_tunnel / 2;
	y_car = 50 +120 - y_tunnel / 2;
	sprite_animate_and_blit(current_time, g_voiture, surf, x_car, y_car, 2);

	if(current_time >= PART6_GFX_START)
	{
		local_time = current_time - PART6_GFX_START;
		t_gfx = local_time / 1000.0f;
		t_gfx = t_gfx > 1.0f ? 1.0f : t_gfx;
		x_gfx = -320 + 320 * t_gfx;
		utils_blit_with_transparency(g_gfx_jaune, surf, x_gfx, 0);
		x_credit = 320 - 160 * t_gfx;
		y_credit = 80;
		font_write("GFX:", x_credit, y_credit, g_font, surf);
		font_write("TAGGY", x_credit+20, y_credit+16, g_font, surf);
	}
	*/
	effects_fade_manage(surf, current_time, part7_fade_timings);
}
