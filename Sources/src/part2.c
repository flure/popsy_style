#include "part2.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "plasma.h"
#include "sprite.h"
#include "font.h"

#define	DDR_LEFT	0
#define	DDR_DOWN	1
#define DDR_UP		2
#define DDR_RIGHT	3

#define DDR_LEFT_X	0
#define DDR_DOWN_X	30
#define	DDR_UP_X	60
#define DDR_RIGHT_X	90

#define DDR_LINE_Y	4

#define PART2_GFX_PSY_START			24675
#define	PART2_GFX_PSY_APPEAR_TIME	1000.0f
#define PART2_DDR_START				16000
#define PART2_DDR_END				26000

extern T_Font *g_font;
static T_Sprite *g_psy_riding;
static T_Sprite *g_psy_jamming;
static SDL_Surface *g_ddr[4];
static SDL_Surface *g_psy_gfx;
static SDL_Surface *g_ddr_empty[4];

static struct t_fade_timing part2_fade_timings[] =
{
	{ PART2_START_TIME, 500, 1},
	{ PART2_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

typedef struct t_ddr_info
{
	int xoffset;
	int arrow;
	unsigned int timing;
} T_DDRInfo;

static T_DDRInfo g_ddr_info[] = {
	{ DDR_LEFT_X,	DDR_LEFT,	250 },
	{ DDR_RIGHT_X,	DDR_RIGHT,	500 },
	{ DDR_DOWN_X,	DDR_DOWN,	750 },
	{ DDR_UP_X,		DDR_UP,		1000 },
	{ DDR_LEFT_X,	DDR_LEFT,	1250 },
	{ DDR_UP_X,		DDR_UP,		1500 },
	{ DDR_RIGHT_X,	DDR_RIGHT,	1750 },
	{ DDR_DOWN_X,	DDR_DOWN,	2000 },
	{ DDR_UP_X,		DDR_UP,		2250 },
	{ DDR_LEFT_X,	DDR_LEFT,	2500 },
	{ DDR_DOWN_X,	DDR_DOWN,	2750 },
	{ DDR_RIGHT_X,	DDR_RIGHT,	3000 }
};

void part2_init(void)
{
	plasma_init();
	g_psy_riding = sprite_create_from_png("data/psy_riding.png", 16, 100, 200, SPRITE_MODE_LOOP);
	g_psy_jamming = sprite_create_from_png("data/psy_jamming.png", 16, 100, 200, SPRITE_MODE_LOOP);
	g_psy_gfx = utils_load_png_to_surface("data/graph_psy.png");
	g_ddr[DDR_UP]	 = utils_load_png_to_surface("data/ddr_up.png");
	g_ddr[DDR_DOWN]  = utils_load_png_to_surface("data/ddr_down.png");
	g_ddr[DDR_LEFT]	 = utils_load_png_to_surface("data/ddr_left.png");
	g_ddr[DDR_RIGHT] = utils_load_png_to_surface("data/ddr_right.png");

	g_ddr_empty[DDR_UP]	 = utils_load_png_to_surface("data/ddr_empty_up.png");
	g_ddr_empty[DDR_DOWN]  = utils_load_png_to_surface("data/ddr_empty_down.png");
	g_ddr_empty[DDR_LEFT]	 = utils_load_png_to_surface("data/ddr_empty_left.png");
	g_ddr_empty[DDR_RIGHT] = utils_load_png_to_surface("data/ddr_empty_right.png");
}

void part2_deinit(void)
{
	int i;
	sprite_destroy(g_psy_riding);
	sprite_destroy(g_psy_jamming);
	SDL_FreeSurface(g_psy_gfx);
	for(i = 0; i < 4; i++) 
	{
		SDL_FreeSurface(g_ddr[i]);
		SDL_FreeSurface(g_ddr_empty[i]);
	}
}

void part2_play(SDL_Surface *surf, unsigned current_time)
{
	int x_psy, y_psy, x_credit, y_credit;
	float time = (current_time - PART2_START_TIME) / 1100.0f;
	float t = (time > 1.0f ? 1.0f : (time < 0.0f ? 0.0f : time));
	float t_gfx = 0.0f;
	float t_ddr = 0.0f, t_ddri, t_ddrline;
	int i, ddrindex, yddr, yddrline;
	

	if(current_time < PART2_START_TIME) return;
	if(current_time > PART2_END_TIME) return;

	time = (current_time - PART2_START_TIME) / 1100.0f;
	t = (time > 1.0f ? 1.0f : (time < 0.0f ? 0.0f : time));
	// Fond : plasma
	plasma_run(surf, current_time);
	plasma_pal_cycle(current_time);

	// Affichage du gfx et du crédit
	if(current_time >= PART2_GFX_PSY_START)
	{
		time = (current_time - PART2_GFX_PSY_START) / PART2_GFX_PSY_APPEAR_TIME;
		t_gfx = (time > 1.0f ? 1.0f : (time < 0.0f ? 0.0f : time));

		utils_blit_with_transparency(g_psy_gfx, surf, -80, 240.0f - 240.0f * t_gfx);	

		x_credit = 320 - t_gfx * 180;
		y_credit = 80;
		font_write("Music:",	 x_credit,	  y_credit,		 g_font, surf);
		font_write("Awesomecat", x_credit+20, y_credit + 16, g_font, surf);
	}

	if((current_time <= PART2_DDR_END + 1000) && (current_time >= PART2_DDR_START))
	{
		if(current_time < (PART2_DDR_END))
		{
			t_ddrline = (current_time - PART2_DDR_START) / 500.0f;
			t_ddrline = t_ddrline > 1.0f ? 1.0f : t_ddrline;
			yddrline = -32 + (32 + DDR_LINE_Y) * t_ddrline;
		}
		else
		{
			t_ddrline = (current_time - PART2_DDR_END) / 500.0f;
			t_ddrline = t_ddrline < 0.0f ? 0.0f : t_ddrline > 1.0f ? 1.0f : t_ddrline;
			yddrline = DDR_LINE_Y - (32 + DDR_LINE_Y) * t_ddrline;
		}
		
		utils_blit_with_transparency(g_ddr_empty[DDR_LEFT], surf,	200 + DDR_LEFT_X, yddrline);
		utils_blit_with_transparency(g_ddr_empty[DDR_DOWN], surf,	200 + DDR_DOWN_X, yddrline);
		utils_blit_with_transparency(g_ddr_empty[DDR_UP], surf,		200 + DDR_UP_X, yddrline);
		utils_blit_with_transparency(g_ddr_empty[DDR_RIGHT], surf,	200 + DDR_RIGHT_X, yddrline);
	}

	// Affichage des flèches DDR
	if((current_time >= PART2_DDR_START))
	{
		t_ddr = (current_time - PART2_DDR_START) / 1000.0f;

		for(i = 0; i < 30; i++)
		{
			ddrindex = i % (sizeof(g_ddr_info) / sizeof(T_DDRInfo));
			t_ddri = t_ddr + i * 0.250f;

			yddr = 1000.0f - t_ddri*100.0f;
			yddr = (yddr < -32 ? 240 : yddr);

			if(yddr >= DDR_LINE_Y)
			{
				utils_blit_with_transparency(g_ddr[g_ddr_info[ddrindex].arrow], surf, 200 + g_ddr_info[ddrindex].xoffset, yddr);
			}

			effects_flash(surf, current_time, PART2_DDR_START + 2750 + i * 250, 100);
		}
	}

	// Affichage du sprite de psy qui danse
	// Psy arrive par la droite, il prend la moitié de l'écran (zoom x2)
	x_psy = 320 * (1.0f - t) + 40 * t - t_gfx * 200;
	y_psy = 60;
	if(t_gfx <= 0.0f) sprite_animate_and_blit(current_time, g_psy_riding, surf, x_psy, y_psy, 2);
	else sprite_animate_and_blit(current_time, g_psy_jamming, surf, x_psy, y_psy, 2);

	effects_fade_manage(surf, current_time, part2_fade_timings);
}
