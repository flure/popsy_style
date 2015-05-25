#include "sprite.h"

#include "SDL.h"

#include <stdlib.h>
#include "utils.h"

T_Sprite* sprite_create_from_png(char *filename, int nb_frames, int frame_height, unsigned frame_duration, int mode)
{
	T_Sprite* sprite = NULL;
	SDL_Surface *surf = NULL;
	surf = utils_load_png_to_surface(filename);
	if(surf == NULL) return NULL;

	sprite = sprite_create_from_surface(surf, nb_frames, frame_height, frame_duration, mode);
	return sprite;
}

T_Sprite* sprite_create_from_surface(SDL_Surface *surf, int nb_frames, int frame_height, unsigned frame_duration, int mode)
{
	T_Sprite* sprite = NULL;
	sprite = malloc(sizeof(T_Sprite));
	if(sprite == NULL) return NULL;

	sprite->nb_frames = nb_frames;
	sprite->frame_duration = frame_duration;
	sprite->frame_width = surf->w;
	sprite->frame_height = frame_height;
	sprite->last_frame_time = 0;
	sprite->frames = surf;
	SDL_SetColorKey(sprite->frames, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0xFF00FF);
	sprite->current_frame = 0;
	sprite->x_pos = 0;
	sprite->y_pos = 0;
	sprite->mode = mode;
	sprite->direction = 1;
	return sprite;
}

void sprite_destroy(T_Sprite* sprite)
{
	if(sprite == NULL) return;
	if(sprite->frames != NULL) SDL_FreeSurface(sprite->frames);
}

void sprite_animate_and_blit(unsigned current_time, T_Sprite *sprite, SDL_Surface *surf, int x, int y, float zoom)
{
	SDL_Rect rectSrc, rectDst;
	int dst_x, dst_y, X, Y;
	float src_x, src_y, zoomstep;
	Uint32 *src_pix, *dst_pix, src_w, dst_w, c;
	if(current_time >= (sprite->last_frame_time + sprite->frame_duration))
	{
		sprite->last_frame_time = current_time;
		switch(sprite->direction)
		{
			default:
			case 1: sprite->current_frame++; break;
			case -1: sprite->current_frame--; break;
		}
		switch(sprite->mode)
		{
			default:
			case SPRITE_MODE_LOOP:
				if((sprite->direction == 1) && (sprite->current_frame >= sprite->nb_frames)) sprite->current_frame = 0;
				if((sprite->direction == -1) && (sprite->current_frame < 0)) sprite->current_frame = sprite->nb_frames - 1;
				break;

			case SPRITE_MODE_PINGPONG:
				if((sprite->direction == 1) && (sprite->current_frame >= sprite->nb_frames)) 
				{
					sprite->direction = -1;
					sprite->current_frame--;
				}
				if((sprite->direction == -1) && (sprite->current_frame < 0)) 
				{
					sprite->direction = 1;
					sprite->current_frame++;
				}
				break;
		}
	}

	rectSrc.x = 0;
	rectSrc.y = sprite->current_frame * sprite->frame_height;
	rectSrc.w = sprite->frame_width;
	rectSrc.h = sprite->frame_height;

	sprite->x_pos = x - sprite->frame_width/2;
	sprite->y_pos = y - sprite->frame_height/2;
	rectDst.x = sprite->x_pos;
	rectDst.y = sprite->y_pos;
	rectDst.w = sprite->frame_width;
	rectDst.h = sprite->frame_height;

	//SDL_BlitSurface(sprite->frames, &rectSrc, surf, &rectDst);
	if(SDL_MUSTLOCK(sprite->frames)) SDL_LockSurface(sprite->frames);
	if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
	dst_pix = (Uint32 *)surf->pixels;
	src_pix = (Uint32 *)sprite->frames->pixels;
	dst_w = surf->pitch / surf->format->BytesPerPixel;
	src_w = sprite->frames->pitch / sprite->frames->format->BytesPerPixel;
	zoomstep = 1.0f / zoom;
	for(src_y = rectSrc.y, dst_y = rectDst.y; src_y < rectSrc.h + rectSrc.y; src_y += zoomstep, dst_y++)
	{
		Y = (int)src_y;
		if((dst_y < 0) || (dst_y > 240)) continue;
		dst_x = rectDst.x;
		for(src_x = rectSrc.x, dst_x = rectDst.x; src_x < rectSrc.w + rectSrc.x; src_x += zoomstep, dst_x++)
		{
			X = (int)src_x;
			if((dst_x < 0) || (dst_x > 320)) continue;

			c = src_pix[X + Y * src_w];
			if((c & 0xFFFFFF) == 0xFF00FF) continue;
			dst_pix[dst_x + dst_y * dst_w] = c;
		}
	}	
	if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
	if(SDL_MUSTLOCK(sprite->frames)) SDL_UnlockSurface(sprite->frames);
}