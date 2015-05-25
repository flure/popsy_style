#include "part6.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "font.h"
#include "sprite.h"
#include "tunnel.h"

#define PART6_GFX_START	70000

extern T_Font *g_font;
static SDL_Surface *g_tunnel_tex;
static SDL_Surface *g_gfx_jaune;
static T_Sprite *g_voiture;

struct t_fade_timing part6_fade_timings[] =
{
	{ PART6_START_TIME, 500, 1}, 
	{ PART6_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

void part6_init(void)
{
	g_tunnel_tex = utils_load_png_to_surface("data/tunnel.png");
	g_gfx_jaune = utils_load_png_to_surface("data/graph_jaune.png");
	g_voiture = sprite_create_from_png("data/voiture.png", 2, 100, 200, SPRITE_MODE_LOOP);
	tunnel_init();
}

void part6_deinit(void)
{
	tunnel_deinit();
	SDL_FreeSurface(g_tunnel_tex);
	SDL_FreeSurface(g_gfx_jaune);
	sprite_destroy(g_voiture);
}

void part6_play(SDL_Surface *surf, unsigned current_time)
{
	unsigned local_time;
	float t_gfx;
	int x_gfx, x_tunnel, y_tunnel, x_car, y_car;
	float x_credit, y_credit;

	if(current_time < PART6_START_TIME) return;
	if(current_time > PART6_END_TIME) return;

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
		font_write("Gfx:", x_credit, y_credit, g_font, surf);
		font_write("tAGGY", x_credit+20, y_credit+16, g_font, surf);
	}

	effects_fade_manage(surf, current_time, part6_fade_timings);
}
