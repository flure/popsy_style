#include "SDL.h"

#include "utils.h"
#include "window.h"
#include "sprite.h"

#include <stdlib.h>
#include <stdio.h>

#define WIDTH		320
#define HEIGHT		240
#define BPP			32
#define FULLSCREEN	0

int parse_commandline(int argc, char **argv, char** filename, int *nb_frames, int *frame_height, unsigned *frame_duration, int *mode)
{
	if(argc < 6)
	{
		printf("Usage : animtester anim nb_frames frame_height frame_duration mode.\n");
		printf("anim           : le fichier .png contenant les frames d'animation\n");
		printf("nb_frames      : le nombre de frames de l'animation\n");
		printf("frame_height   : la hauteur en pixels d'une frame\n");
		printf("frame_duration : la durée de chaque frame en ms\n");
		printf("mode           : L pour faire boucler (loop) l'animation, P pour faire des allers-retours (pingpong)\n");
		return 0;
	}

	*filename = argv[1];
	*nb_frames = atoi(argv[2]);
	*frame_height = atoi(argv[3]);
	*frame_duration = (unsigned)atoi(argv[4]);
	if((argv[5][0] == 'L') || (argv[5][0] == 'l')) *mode = SPRITE_MODE_LOOP;
	else if((argv[5][0] == 'P') || (argv[5][0] == 'p')) *mode = SPRITE_MODE_PINGPONG;
	else *mode = SPRITE_MODE_LOOP;
	return 1;
}

int main(int argc, char** argv)
{
	T_Sprite *sprite;
	int current_time;
	char* filename;
	int nb_frames, frame_height;
	unsigned frame_duration;
	int mode;


	// animtester anim.png nb_frames frame_height frame_duration
	if(!parse_commandline(argc, argv, &filename, &nb_frames, &frame_height, &frame_duration, &mode)) return 0;

	window_open(FULLSCREEN, "POPSY STYLE");

	//img = easypng_load("data/anims/mec_jaune/mec_jaune_01.png");
	//test = utils_load_png_to_surface("data/test_anim.png");
	sprite = sprite_create_from_png(filename, nb_frames, frame_height, frame_duration, mode);
	// sprite = sprite_create_from_png(argv[1], 4, 120, 200);
	// if(sprite == NULL)
	// {
	// 	printf("Impossible de charger l'animation.\n");
	// 	window_close();
	// 	return 0;
	// }


	while(!window_is_closed())
	{
		SDL_FillRect(g_screen, NULL, 0);
		current_time = SDL_GetTicks();
		sprite_animate_and_blit(current_time, sprite, g_screen, 0, 0, 1);
		//SDL_UpdateRect(g_screen, 0, 0, 0, 0);
		SDL_Flip(g_screen);
	}

	window_close();
	return 0;
}