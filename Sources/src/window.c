#include "window.h"

#include <IL/il.h>

#define WIDTH		320
#define HEIGHT		240
#define BPP			32
#define RES_FACTOR	2

SDL_Surface* g_screen = NULL;
SDL_Surface* g_real_screen = NULL;

void window_open(int fullscreen, char *title)
{
	unsigned flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
	if(fullscreen) flags |= SDL_FULLSCREEN;
	SDL_Init(SDL_INIT_VIDEO);
	g_real_screen = SDL_SetVideoMode(WIDTH*RES_FACTOR, HEIGHT*RES_FACTOR, BPP, flags);
	SDL_WM_SetCaption(title, NULL);
	SDL_ShowCursor(SDL_DISABLE);
	g_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, BPP, g_real_screen->format->Rmask, g_real_screen->format->Gmask, g_real_screen->format->Bmask, g_real_screen->format->Amask);
	ilInit();
}

int window_is_closed(void)
{
	SDL_Event event;
	while( SDL_PollEvent( &event ) )
	{
		if((event.type == SDL_QUIT) || 
			((event.type == SDL_KEYUP) && 
				(event.key.keysym.sym == SDLK_ESCAPE))) return 1;
  	}
  	return 0;
}

void window_close(void)
{
	SDL_FreeSurface(g_screen);
	SDL_Quit();
}

void window_flip(void)
{
	int rs_w = g_real_screen->pitch / g_real_screen->format->BytesPerPixel;
	int s_w = g_screen->pitch / g_screen->format->BytesPerPixel;
	int s_x, s_y, rs_x, rs_y;
	Uint32 *rs_pixels = (Uint32 *)(g_real_screen->pixels);
	Uint32 *s_pixels = (Uint32 *)(g_screen->pixels);
	// copie de g_screen sur g_real_screen
	//SDL_BlitSurface(g_screen, NULL, g_real_screen, NULL);
	if(SDL_MUSTLOCK(g_real_screen)) SDL_LockSurface(g_real_screen);
	for(rs_y = 0; rs_y < HEIGHT * RES_FACTOR; rs_y++)
	{
		s_y = rs_y / RES_FACTOR;
		for(rs_x = 0; rs_x < WIDTH * RES_FACTOR; rs_x++)
		{
			s_x = rs_x / RES_FACTOR;
			rs_pixels[rs_x + rs_y * rs_w] = s_pixels[s_x + s_y * s_w];
		}
	}
	if(SDL_MUSTLOCK(g_real_screen)) SDL_UnlockSurface(g_real_screen);

	// flip de g_real_screen
	SDL_Flip(g_real_screen);
}