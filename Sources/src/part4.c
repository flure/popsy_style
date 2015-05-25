#include "part4.h"

#include <math.h>
#include "SDL.h"
#include "effects.h"
#include "utils.h"
#include "font.h"
#include "sprite.h"
#include "window.h"
#include "starfield.h"

extern T_Font *g_font;

static struct t_flash_timing part4_flash_timings[] =
{
	{ 0, 0 }
};

struct t_fade_timing part4_fade_timings[] =
{
	{ PART4_START_TIME, 500, 1}, 
	{ PART4_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

static SDL_Surface *g_starfield;
static SDL_Surface *g_greetsurf;
static int *g_lut1;
static int *g_lut2;

#define NB_GREETS		19
static SDL_Surface *g_greets[NB_GREETS];
static int g_greetstimings[NB_GREETS+1] = 
{
	PART4_START_TIME + 300,
	PART4_START_TIME + 300*2,
	PART4_START_TIME + 300*3,
	PART4_START_TIME + 300*4,
	PART4_START_TIME + 300*5,
	PART4_START_TIME + 300*6,
	PART4_START_TIME + 300*7,
	PART4_START_TIME + 300*8,
	PART4_START_TIME + 300*9,

	PART4_START_TIME + 300*9 + 200,
	PART4_START_TIME + 300*9 + 200*2,
	PART4_START_TIME + 300*9 + 200*3,
	PART4_START_TIME + 300*9 + 200*4,
	PART4_START_TIME + 300*9 + 200*5,
	PART4_START_TIME + 300*9 + 200*6,
	PART4_START_TIME + 300*9 + 200*7,
	PART4_START_TIME + 300*9 + 200*8,
	PART4_START_TIME + 300*9 + 200*9,
	PART4_START_TIME + 300*9 + 200*10,
	PART4_END_TIME
};

#define GREETS_SFX			0
#define GREETS_XMEN			1
#define GREETS_PF			2
#define GREETS_LNX			3
#define GREETS_VM			4
#define GREETS_TRIPLEA		5
#define GREETS_MAP			6
#define GREETS_CAT			7
#define GREETS_REBELS		8

#define GREETS_CDX			9
#define GREETS_CYBERNOSTRA	10
#define GREETS_KIKIPROD		11
#define GREETS_SECTORONE	12
#define GREETS_TRSI			13
#define GREETS_RAZOR1911	14
#define GREETS_TPOLM		15
#define GREETS_FRQ			16
#define GREETS_BOMB			17
#define GREETS_STYLE		18


void part4_init(void)
{
	int i;
	g_starfield = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	g_greetsurf = SDL_CreateRGBSurface(SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	g_greets[GREETS_SFX] = utils_load_png_to_surface("data/sfx.png");
	g_greets[GREETS_XMEN] = utils_load_png_to_surface("data/xmen.png");
	g_greets[GREETS_PF] = utils_load_png_to_surface("data/punkfloyd.png");
	g_greets[GREETS_LNX] = utils_load_png_to_surface("data/lnx.png");	
	g_greets[GREETS_VM] = utils_load_png_to_surface("data/vitalmotion.png");
	g_greets[GREETS_TRIPLEA] = utils_load_png_to_surface("data/triplea.png");
	g_greets[GREETS_MAP] = utils_load_png_to_surface("data/map.png");
	g_greets[GREETS_CAT] = utils_load_png_to_surface("data/ctrlalttest.png");
	g_greets[GREETS_REBELS] = utils_load_png_to_surface("data/rebelz.png");
	g_greets[GREETS_CDX] = utils_load_png_to_surface("data/kalodox.png");
	g_greets[GREETS_CYBERNOSTRA] = utils_load_png_to_surface("data/cybernostra.png");
	g_greets[GREETS_KIKIPROD] = utils_load_png_to_surface("data/kikiprod.png");
	g_greets[GREETS_SECTORONE] = utils_load_png_to_surface("data/sectorone.png");
	g_greets[GREETS_TRSI] = utils_load_png_to_surface("data/trsi.png");
	g_greets[GREETS_RAZOR1911] = utils_load_png_to_surface("data/razor.png");
	g_greets[GREETS_TPOLM] = utils_load_png_to_surface("data/tpolm.png");
	g_greets[GREETS_FRQ] = utils_load_png_to_surface("data/frequency.png");
	g_greets[GREETS_BOMB] = utils_load_png_to_surface("data/bomb.png");
	g_greets[GREETS_STYLE] = utils_load_png_to_surface("data/style.png");

	g_lut1 = (int *)malloc(512*512*2*sizeof(int));
	g_lut2 = (int *)malloc(512*512*2*sizeof(int));
	deform_lut_a(g_lut1);
	deform_lut_c(g_lut2);
	starfield_init();
	SDL_FillRect(g_greetsurf, NULL, 0);
}

void part4_deinit(void)
{
	int i;
	starfield_deinit();
	for(i = 0; i < NB_GREETS; i++) SDL_FreeSurface(g_greets[i]);
	SDL_FreeSurface(g_starfield);
	SDL_FreeSurface(g_greetsurf);
}

void part4_play(SDL_Surface *surf, unsigned current_time)
{
	SDL_Rect rect;
	unsigned local_time;
	int i, x, y;
	if(current_time < PART4_START_TIME) return;
	if(current_time > PART4_END_TIME) return;

	SDL_FillRect(surf, NULL, 0);

	local_time = current_time - PART4_START_TIME;

	starfield_run(g_starfield, 160 + 40 * sin(current_time / 1000.0f), 120 + 40 * sin(current_time / 1000.0f), -0.01f);
	effects_blur(g_starfield);
	effects_blur(g_greetsurf);
	
	for(i = 0; i < NB_GREETS+1; i++)
	{
		if((current_time >= g_greetstimings[i]) && (current_time < g_greetstimings[i+1]))
		{
			utils_blit_with_transparency(g_greets[i], g_greetsurf, 0, 0);
		}
	}
	
	SDL_BlitSurface(g_starfield, NULL, surf, NULL);
	SDL_BlitSurface(g_greetsurf, NULL, surf, NULL);

	for(i = 0; i < NB_GREETS; i++)
	{
		effects_flash(surf, current_time, g_greetstimings[i], 100);
	}
	effects_fade_manage(surf, current_time, part4_fade_timings);
}
