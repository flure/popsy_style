#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

#define SPRITE_MODE_LOOP		0
#define SPRITE_MODE_PINGPONG	1

typedef struct t_sprite
{
	SDL_Surface* frames;
	int nb_frames;
	int current_frame;
	int frame_width;
	int frame_height;
	unsigned frame_duration;
	unsigned last_frame_time;
	int x_pos;
	int y_pos;
	int mode; // voir les define SPRITE_MODE_* plus haut
	int direction; // sens de l'anim : 1 en avant, -1 en arrière
} T_Sprite;

T_Sprite* sprite_create_from_png(char *filename, int nb_frames, int frame_height, unsigned frame_duration, int mode);
T_Sprite* sprite_create_from_surface(SDL_Surface *surf, int nb_frames, int frame_height, unsigned frame_duration, int mode);
void sprite_destroy(T_Sprite* sprite);

void sprite_animate_and_blit(unsigned current_time, T_Sprite *sprite, SDL_Surface *surf, int x, int y, float zoom);



#endif // SPRITE_H