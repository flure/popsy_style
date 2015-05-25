#include "effects.h"

#include "SDL.h"
#include <math.h>

#include "utils.h"

struct t_flashsurf_timing *g_flashsurf_timings = NULL; // Pour l'init, voir dans _flash_surf_init()

// -- Variables globales des effets --------------------------------------------
static SDL_Surface *g_flash_surf = NULL;
static SDL_Surface *g_fade_surf = NULL;
static SDL_Surface *g_flashsurf_surf = NULL;
static SDL_Surface *g_statics_surf = NULL;


void effects_destroy_flashsurf_timing_table(struct t_flashsurf_timing_table *table)
{
	int i;
	for(i = 0; i < table->nb_surfs; i++)
	{
		SDL_FreeSurface(table->surfs[i]);
	}
	free(table->surfs);
	free(table->timings);
	free(table);
}

// -- Init des effets ----------------------------------------------------------
void _flash_init(void)
{
	g_flash_surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
}

void _fade_init(void)
{
	g_fade_surf  = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
}

void _flash_surf_init(void)
{
	g_flashsurf_surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
}

void _statics_init(void)
{
	g_statics_surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 320, 240, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
}

// -- init globale -------------------------------------------------------------
void effects_init(void)
{
	_flash_init();
	_fade_init();
	_flash_surf_init();
	_statics_init();
}

// -- deinit des effets --------------------------------------------------------
void _flash_deinit(void)
{
	SDL_FreeSurface(g_flash_surf);
}

void _fadein_deinit(void)
{
	SDL_FreeSurface(g_fade_surf);
}

void _statics_deinit(void)
{
	SDL_FreeSurface(g_statics_surf);
}

void _flash_surf_deinit(void)
{
	SDL_FreeSurface(g_flashsurf_surf);
}
// -- deinit globale -----------------------------------------------------------
void effects_deinit(void)
{
	_flash_deinit();
	_fadein_deinit();
	_flash_surf_deinit();
}

// -- les effets ---------------------------------------------------------------
void effects_flash(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration)
{
	float t, amount;
	unsigned char alpha;
	unsigned color;
	if(current_time < (start_time - duration/2)) return;
	if(current_time > (start_time + duration/2)) return;

	t = (current_time - start_time) / (float)duration;
	t = SMOOTHSTEP(t);
	amount = sinf(_M_PI * t);
	alpha = (unsigned char)(255 * amount);
	color = (alpha << 24) | 0xFFFFFF;

	SDL_FillRect(g_flash_surf, NULL, color);
	SDL_BlitSurface(g_flash_surf, NULL, surf, NULL);
}

void effects_flash_manage(SDL_Surface *surf, unsigned current_time, struct t_flash_timing *timings)
{
	int i;
	for(i = 0; ; i++)
	{
		if((timings[i].start_time == 0) && (timings[i].duration == 0)) return;
		if(current_time < timings[i].start_time) continue;
		if(current_time > (timings[i].start_time + timings[i].duration)) continue;

		effects_flash(surf, current_time, timings[i].start_time, timings[i].duration);
	}
}

void effects_fadein(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration)
{
	float t;
	unsigned char alpha;
	unsigned color;
	if(current_time < start_time) return;
	if(current_time > (start_time + duration)) return;
	t = (current_time - start_time) / (float)duration;
	t = SMOOTHSTEP(t);
	alpha = (unsigned char)(255 * (1.0f - t));
	color = (alpha << 24) & 0xFF000000;	

	SDL_FillRect(g_fade_surf, NULL, color);
	SDL_BlitSurface(g_fade_surf, NULL, surf, NULL);
}

void effects_fadeout(SDL_Surface *surf, unsigned current_time, unsigned start_time, unsigned duration)
{
	float t;
	unsigned char alpha;
	unsigned color;
	if(current_time < start_time) return;
	if(current_time > (start_time + duration)) return;
	t = (current_time - start_time) / (float)duration;
	t = SMOOTHSTEP(t);
	alpha = (unsigned char)(255 * (t));
	color = (alpha << 24) & 0xFF000000;	

	SDL_FillRect(g_fade_surf, NULL, color);
	SDL_BlitSurface(g_fade_surf, NULL, surf, NULL);
}

void effects_fade_manage(SDL_Surface *surf, unsigned current_time, struct t_fade_timing *timings)
{
	int i;
	for(i = 0; ; i++)
	{
		if((timings[i].start_time == 0) && (timings[i].duration == 0)) return;
		if(current_time < timings[i].start_time) continue;
		if(current_time > (timings[i].start_time + timings[i].duration)) continue;
		
		if(timings[i].direction > 0)
		{
			effects_fadein(surf, current_time,  timings[i].start_time, timings[i].duration);
		}
		else
		{
			effects_fadeout(surf, current_time, timings[i].start_time, timings[i].duration);
		}
	}
}

void effects_flash_surf(SDL_Surface *src, SDL_Surface *dest, unsigned current_time, unsigned start_time, unsigned duration, int pos_x, int pos_y)
{
	float t, amount;
	unsigned char alpha;
	unsigned color;
	int s_w, s_h, d_w, d_h, s_wp, d_wp;
	int x, y;
	SDL_Rect rect = { 0, 0, 0, 0 };
	Uint32 *s_pix, *fs_pix;
	if(current_time < start_time) return;
	if(current_time > (start_time + duration)) return;

	t = (current_time - start_time) / (float)duration;
	t = SMOOTHSTEP(t);
	amount = sinf(_M_PI * t);
	alpha = (unsigned char)(256 * amount);
	//color = (alpha << 24) | 0xFFFFFF;

	s_wp = src->pitch / src->format->BytesPerPixel;
	d_wp = g_flashsurf_surf->pitch / g_flashsurf_surf->format->BytesPerPixel;
	s_w = src->w;
	d_w = dest->w;
	s_h = src->h;
	d_h = dest->h;
	SDL_FillRect(g_flashsurf_surf, NULL, 0xFF0000FF);
	if(SDL_MUSTLOCK(src)) SDL_LockSurface(src);
	if(SDL_MUSTLOCK(g_flashsurf_surf)) SDL_LockSurface(g_flashsurf_surf);
	s_pix = (Uint32 *)(src->pixels);
	fs_pix = (Uint32 *)(g_flashsurf_surf->pixels);
	for(y = 0; (y < s_h) && (y < d_h);  y++)
	{
		for(x = 0; (x < s_w) && (x < d_w); x++)
		{
			color = s_pix[x + y * s_wp];
			fs_pix[x + y *d_wp] = (alpha<<24) | color;
		}
	}
	if(SDL_MUSTLOCK(g_flashsurf_surf)) SDL_UnlockSurface(g_flashsurf_surf);
	if(SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
	rect.x = pos_x;
	rect.y = pos_y;
	rect.w = 320;
	rect.h = 240;
	SDL_BlitSurface(g_flashsurf_surf, NULL, dest, &rect);
}

void effects_flashsurf_manage(SDL_Surface *dest, unsigned current_time, struct t_flashsurf_timing_table *timings_table)
{
	int i;
	if(timings_table == NULL) return;
	for(i = 0; i < timings_table->nb_timings; i++)
	{
		if((timings_table->timings[i].start_time == 0) && (timings_table->timings[i].duration == 0)) return;
		if(current_time < timings_table->timings[i].start_time) continue;
		if(current_time > (timings_table->timings[i].start_time + timings_table->timings[i].duration)) continue;
		
		effects_flash_surf(timings_table->timings[i].surf, 
							dest, 
							current_time, 
							timings_table->timings[i].start_time, timings_table->timings[i].duration, 
							timings_table->timings[i].pos_x, timings_table->timings[i].pos_y);
	}
}


void effects_statics(SDL_Surface *dest, unsigned current_time)
{
	int x, y, w;
	Uint32 *pix, c, r, g, b, color;

	if(SDL_MUSTLOCK(g_statics_surf)) SDL_LockSurface(g_statics_surf);
	pix = (Uint32 *)g_statics_surf->pixels;
	w = g_statics_surf->pitch / g_statics_surf->format->BytesPerPixel;
	for(y = 0; y < 240; y++)
	{
		for(x = 0; x < 320; x++)
		{
			c = rand()%256;
			r = (Uint32)(c*0.95f) & 0xFF;
			g = (Uint32)(c*0.95f) & 0xFF;
			b = (Uint32)(c*1.0f) & 0xFF;
			color = r + (g << 8) + (b << 16);
			pix[x + y*w] = (0xFF << 24) | (color & 0xFFFFFF);
		}
	}
	if(SDL_MUSTLOCK(g_statics_surf)) SDL_UnlockSurface(g_statics_surf);
	
	SDL_BlitSurface(g_statics_surf, NULL, dest, NULL);
}

void effects_blur(SDL_Surface *dest)
{
	int		x, y;
	Uint32	*pix, offset, w, c0, c1, c2;
	Uint32	r, g, b, color, a;

	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	pix = (Uint32 *)dest->pixels;
	w = dest->pitch / dest->format->BytesPerPixel;
	// passe horizontale
	for(y = 0; y < 240; y++)
	{
		for(x = 0; x < 320; x++)
		{
			offset = x + y*w;
			c0 = pix[offset];
			if(x < 319) c1 = pix[offset+1];
			else c1 = 0;
			if(x > 0) c2 = pix[offset-1];
			else c2 = 0;

			r = ((c0 >> 16) & 0xFF) + ((c1 >> 16) & 0xFF) + ((c2 >> 16) & 0xFF);
			g = ((c0 >> 8) & 0xFF) + ((c1 >> 8) & 0xFF) + ((c2 >> 8) & 0xFF);
			b = ((c0) & 0xFF) + ((c1) & 0xFF) + ((c2) & 0xFF);

			r /= 3;
			g /= 3;
			b /= 3;

			a = (r + g + b) / 3;

			color = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));

			pix[offset] = ((a&0xFF) << 24) | (color & 0xFFFFFF);
		}
	}
	// passe verticale
	for(y = 0; y < 240; y++)
	{
		for(x = 0; x < 320; x++)
		{
			offset = x + y*w;
			c0 = pix[offset];
			if(y < 239) c1 = pix[offset+w];
			else c1 = 0;
			if(y > 0) c2 = pix[offset-w];
			else c2 = 0;

			r = ((c0 >> 16) & 0xFF) + ((c1 >> 16) & 0xFF) + ((c2 >> 16) & 0xFF);
			g = ((c0 >> 8) & 0xFF) + ((c1 >> 8) & 0xFF) + ((c2 >> 8) & 0xFF);
			b = ((c0) & 0xFF) + ((c1) & 0xFF) + ((c2) & 0xFF);

			r /= 3;
			g /= 3;
			b /= 3;

			a = (r + g + b) / 3;

			color = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));

			pix[offset] = ((a&0xFF) << 24) | (color & 0xFFFFFF);
		}
	}
	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
}

void effects_blur_horizontal(SDL_Surface *dest, int nrepeat)
{
	int		x, y, i;
	Uint32	*pix, offset, w, c0, c1, c2;
	Uint32	r, g, b, color, a;

	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	pix = (Uint32 *)dest->pixels;
	w = dest->pitch / dest->format->BytesPerPixel;
	
	for(y = 0; y < 240; y++)
	{
		for(x = 0; x < 320; x++)
		{
			for(i = 0; i < nrepeat; i++)
			{
				offset = x + y*w;
				c0 = pix[offset];
				if(x < 319) c1 = pix[offset+1];
				else c1 = 0;
				if(x > 0) c2 = pix[offset-1];
				else c2 = 0;
				r = ((c0 >> 16) & 0xFF) + ((c1 >> 16) & 0xFF) + ((c2 >> 16) & 0xFF);
				g = ((c0 >> 8) & 0xFF) + ((c1 >> 8) & 0xFF) + ((c2 >> 8) & 0xFF);
				b = ((c0) & 0xFF) + ((c1) & 0xFF) + ((c2) & 0xFF);

				r /= 3;
				g /= 3;
				b /= 3;

				a = (r + g + b) / 3;

				color = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));

				pix[offset] = ((a&0xFF) << 24) | (color & 0xFFFFFF);
			}
		}
	}
	
	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
}

void effects_draw_scroll_band(SDL_Surface *surf, int height, float opacity)
{
#define BORDER_WIDTH	6
	int x, y, Y;
	Uint32 *pix, w;
	Uint32 c, color, r, g, b;

	opacity = (opacity > 1.0F ? 1.0f : (opacity < 0.0f ? 0.0f : opacity));

	if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
	pix = (Uint32 *)surf->pixels;
	w = surf->pitch / surf->format->BytesPerPixel;
	for(y = 240 - height; y < 240; y++)
	{
		Y = y - 240 + height;
		for(x = 0; x < 320; x++)
		{
			color = 0;
			if(opacity)
			{
				color = pix[x + y * w];
				r = color & 0xFF;
				g = (color >> 8) & 0xFF;
				b = (color >> 16) & 0xFF;

				r *= (1.0f - opacity);
				g *= (1.0f - opacity);
				b *= (1.0f - opacity);
				color = (b<<16) | (g<<8) | r;
			}
			
			if(Y < BORDER_WIDTH)
			{
				c = (Uint32)(255 * sinf((Y+1) * _M_PI/BORDER_WIDTH)) & 0xFF;			
				color = (c<<16) | (c<<8) | c;
			}			

			pix[x + y * w] = 0xFF000000 | color;
		}
	}
	if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
}