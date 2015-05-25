#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"

extern SDL_Surface* g_screen;

void window_open(int fullscreen, char *title);
int window_is_closed(void);
void window_close(void);
void window_flip(void);

#endif // WINDOW_H