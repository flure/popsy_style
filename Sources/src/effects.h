#ifndef EFFECTS_H
#define EFFECTS_H

#include "deform.h"

#include "SDL.h"


struct t_flash_timing
{
	unsigned start_time;
	unsigned duration;
};

struct t_fade_timing
{
	unsigned start_time;
	unsigned duration;
	int direction;
};

struct t_flashsurf_timing
{
	unsigned start_time;
	unsigned duration;
	SDL_Surface *surf;
	int pos_x;
	int pos_y;
};

struct t_flashsurf_timing_table
{
	SDL_Surface **surfs;
	int nb_surfs;
	struct t_flashsurf_timing *timings;
	int nb_timings;
};

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

void effects_init(void);
void effects_deinit(void);

void effects_flash(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration);
void effects_flash_manage(SDL_Surface *surf, unsigned current_time, struct t_flash_timing *timings);

void effects_fadein(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration);
void effects_fadeout(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration);
void effects_fade_manage(SDL_Surface *surf, unsigned current_time, struct t_fade_timing *timings);

void effects_flash_surf(SDL_Surface *src, SDL_Surface *dest, unsigned current_time, unsigned start_time, unsigned duration, int pos_x, int pos_y);
void effects_flashsurf_manage(SDL_Surface *dest, unsigned current_time, struct t_flashsurf_timing_table *timings_table);

void effects_statics(SDL_Surface *dest, unsigned current_time);

void effects_blur(SDL_Surface *dest);
void effects_blur_horizontal(SDL_Surface *dest, int nrepeat);

void effects_destroy_flashsurf_timing_table(struct t_flashsurf_timing_table *table);

void effects_draw_scroll_band(SDL_Surface *surf, int height, float opacity);

#endif // EFFECTS_H