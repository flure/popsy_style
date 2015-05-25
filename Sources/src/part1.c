#include "part1.h"

#include "SDL.h"
#include "effects.h"
#include "utils.h"

#define TITLE_APPEAR_TIME	5000

static SDL_Surface *g_title;

static struct t_flash_timing part1_flash_timings[] =
{
	{ 0, 0 }
};

static struct t_fade_timing part1_fade_timings[] =
{
	{ 0, 1000, 1 },
	{ TITLE_APPEAR_TIME-500, 500, -1},
	{ TITLE_APPEAR_TIME, 500, 1},
	{ PART1_END_TIME-500, 500, -1}, 
	{ 0, 0}
};

static struct t_flashsurf_timing_table *part1_flashsurf_timings;

void part1_init(void)
{
	int i;

	g_title = utils_load_png_to_surface("data/title.png");
	part1_flashsurf_timings = (struct t_flashsurf_timing_table *)malloc(sizeof(struct t_flashsurf_timing_table));
	part1_flashsurf_timings->nb_surfs = 1;
	part1_flashsurf_timings->surfs = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * part1_flashsurf_timings->nb_surfs);
	part1_flashsurf_timings->surfs[0] = utils_load_png_to_surface("data/flash.png");
	part1_flashsurf_timings->nb_timings = 31;
	part1_flashsurf_timings->timings = (struct t_flashsurf_timing *)malloc(sizeof(struct t_flashsurf_timing) * part1_flashsurf_timings->nb_timings);

	for(i = 0; i < part1_flashsurf_timings->nb_timings; i++)
	{
		part1_flashsurf_timings->timings[i].start_time = 400*(i+1);
		part1_flashsurf_timings->timings[i].duration   = 200;
		part1_flashsurf_timings->timings[i].pos_x = 0;
		part1_flashsurf_timings->timings[i].pos_y = 0;
		part1_flashsurf_timings->timings[i].surf = part1_flashsurf_timings->surfs[0];
	}
}

void part1_deinit(void)
{
	SDL_FreeSurface(g_title);
	effects_destroy_flashsurf_timing_table(part1_flashsurf_timings);
}

void part1_play(SDL_Surface *surf, unsigned current_time)
{
	if(current_time < PART1_START_TIME) return;
	if(current_time > PART1_END_TIME) return;

	if(current_time < TITLE_APPEAR_TIME)
	{
		// Neige
		effects_statics(surf, current_time);
	}
	else
	{
		utils_blit_with_transparency(g_title, surf, 0, 0);
	}

	// Neige qui apparaît en fondu
	effects_fade_manage(surf, current_time, part1_fade_timings);

	
	// Flash avec logo popsy style
	effects_flashsurf_manage(surf, current_time, part1_flashsurf_timings);

	// Flash final pour passer à la part2
}
