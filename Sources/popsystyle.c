#include "SDL.h"
#include "SDL_main.h"

#include "sprite.h"
#include "utils.h"
#include "window.h"
#include "effects.h"
#include "music.h"
#include "interference.h"
#include "starfield.h"
#include "plasma.h"
#include "font.h"
#include "tunnel.h"

#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"
#include "part5.h"
#include "part6.h"
#include "part7.h"

#include <math.h>

#define FULLSCREEN	0

T_Font *g_font;

char *g_scroll_text = 
"Oh hay! Hello TUM pals! Hope you enjoy our little gangnam demo :) "
"Awesome cat is awesome, thank you so much dude for the track! "
"Yeah we know we dont give news, its been a long time since our last production, but hey, PoPsYs are still up and chilling! "
"As you can see we are always up for partying and making fun demos! "
"Almost every gfx have been done at the party place. "
"We love everyone, even you that we forgot (and we are so so sorry about that!) <3 "
"The ugly code was almost ready but you know there is always something missing... "
"Well we hope you enjoy it, we made it with a lot of love for you <3 "
"Dont forget to come at the VIP in 2013. Stay tuned for more infos! "
"Still reading? really?! Go dance! "
"So you are still reading. Why don't you dance? Are you shy? You dont dance the Gangnam Style? "
"Come on, lets have some fun dudes! "
"Ok now it's the part where we have no more ideas... ;) "
"TUM was a great party! We hope to see you soon again! Why not at Revision?";


int main(int argc, char **argv)
{
	int current_time, start_time, y_scroll_band;
	float t_scroll;
	SDL_Surface *tunnel_tex;
	
	window_open(FULLSCREEN, "POPSY STYLE");

	music_init();
	effects_init();
	part1_init();
	part2_init();
	part3_init();
	part4_init();
	part5_init();
	part6_init();
	part7_init();
	plasma_init();
	tunnel_init();
	g_font = font_init("data/fonte3.png");
	music_start("data/gangnam.ogg");
	start_time = SDL_GetTicks();

	while(!window_is_closed())
	{
		current_time = SDL_GetTicks() - start_time;

//current_time += PART2_START_TIME;

		if(current_time >= PART7_END_TIME) 
		{
			window_close();
			break;
		}

		part1_play(g_screen, current_time);
		part2_play(g_screen, current_time);
		part3_play(g_screen, current_time);
		part4_play(g_screen, current_time);
		part5_play(g_screen, current_time);
		part6_play(g_screen, current_time);
		part7_play(g_screen, current_time);
		
		if(current_time >= PART2_START_TIME)
		{
			t_scroll = (current_time - PART2_START_TIME - 1000) / 1000.0f;
			t_scroll = (t_scroll > 1.0f ? 1.0f : (t_scroll < 0.0f ? 0.0f : t_scroll));
			y_scroll_band = 24 * t_scroll;
		}
		if(current_time >= PART7_END_TIME - 3800)
		{
			t_scroll = 1.0f - (current_time - PART7_END_TIME + 3800) / 1000.0f;
			t_scroll = (t_scroll > 1.0f ? 1.0f : (t_scroll < 0.0f ? 0.0f : t_scroll));
			y_scroll_band = 24 * t_scroll;
		}

		effects_draw_scroll_band(g_screen, y_scroll_band, 0.7);
		

		if(current_time >= PART2_START_TIME+2000)
		{
			font_write(g_scroll_text, 500-(current_time-PART2_START_TIME-1000) / 8.0f, 221, g_font, g_screen);
		}

		window_flip();
	}

	music_stop();
	music_deinit();
	
	/*
	part7_deinit();
	part6_deinit();
	part5_deinit();
	part4_deinit();
	part3_deinit();
	part2_deinit();
	part1_deinit();
	
	font_deinit(g_font);
	effects_deinit();
	
	window_close();*/
	return 0;
}