#include "part3.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "font.h"
#include "sprite.h"

extern T_Font *g_font;
static T_Sprite *g_hyuna_riding;
static int *g_lut1, *g_lut2;
static SDL_Surface *g_deform_surf;
static SDL_Surface *g_gfx_hyuna;

#define PART3_CREDIT_START	35000
#define PART3_GFX_HYUNA		35000

#define PART3_HYUNA_ZOOM	30000

struct t_fade_timing part3_fade_timings[] =
{
	{ PART3_START_TIME, 500, 1}, 
	{ PART3_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

void part3_init(void)
{
	g_hyuna_riding = sprite_create_from_png("data/hyuna_riding.png", 16, 100, 200, SPRITE_MODE_LOOP);
	g_deform_surf = utils_load_png_to_surface("data/test_deform.png");
	g_gfx_hyuna = utils_load_png_to_surface("data/graph_hyuna.png");
	g_lut1 = (int *)malloc(512*512*2*sizeof(int));
	g_lut2 = (int *)malloc(512*512*2*sizeof(int));
	deform_lut_a(g_lut1);
	deform_lut_c(g_lut2);
}

void part3_deinit(void)
{
	free(g_lut1);
	free(g_lut2);
	SDL_FreeSurface(g_deform_surf);
	SDL_FreeSurface(g_gfx_hyuna);
	sprite_destroy(g_hyuna_riding);
}

void part3_play(SDL_Surface *surf, unsigned current_time)
{
	float time, zoom_hyuna, t_zoom;
	int x_hyuna, y_hyuna, x_credit, y_credit, x_gfx;
	if(current_time < PART3_START_TIME) return;
	if(current_time > PART3_END_TIME) return;

	time = current_time / 1000.0f;

	if(current_time < PART3_GFX_HYUNA)
	{
		t_zoom = (current_time - 1000 - PART3_START_TIME) / 1000.0f;
		t_zoom = t_zoom > 1.0f ? 1.0f : (t_zoom < 0.0f ? 0.0f : t_zoom); 
		x_hyuna = 80 + 35 * (1.0f - t_zoom);
		y_hyuna = 80 + 80 * (1.0f - t_zoom);
	}
	else
	{
		t_zoom = 1.0f - (current_time - PART3_GFX_HYUNA) / 1000.0f;
		t_zoom = t_zoom > 1.0f ? 1.0f : (t_zoom < 0.0f ? 0.0f : t_zoom);
		x_hyuna = 80 + 35 * (1.0f - t_zoom);
		y_hyuna = 80 + 80 * (1.0f - t_zoom);
	}
	zoom_hyuna = t_zoom * 2.0f;
	deform_draw_with_lut_2(current_time/5.0f, g_deform_surf, surf, g_lut1, g_lut2);
	
	// Affichage du gfx et du crédit
	if(current_time >= PART3_CREDIT_START)
	{
		time = (current_time - PART3_CREDIT_START) / 1000.0f;
		time = (time > 1.0f ? 1.0f : time);
		x_credit = 4;
		y_credit = -32 + time * 100;
		font_write("Pixels:",	  x_credit,    y_credit,	  g_font, surf);
		font_write("TiteiKo", x_credit+20, y_credit + 16, g_font, surf);

		x_gfx = 360 - time * 360;
		utils_blit_with_transparency(g_gfx_hyuna, surf, x_gfx, 0);
	}

	if(current_time >= PART3_START_TIME+1000)
	{
		sprite_animate_and_blit(current_time, g_hyuna_riding, surf, x_hyuna, y_hyuna, zoom_hyuna);
	}

	effects_fade_manage(surf, current_time, part3_fade_timings);
}
