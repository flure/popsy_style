#include "utils.h"

#include <IL/il.h>
#include "SDL.h"
#include "window.h"

SDL_Surface* utils_load_png_to_surface(char *filename)
{
	SDL_Surface *result = NULL, *tmp;
	int pitch;
	ILuint image_id;
	ILuint width, height, bpp;
	ILubyte *bytes;
	
	ilGenImages(1, &image_id);
	ilBindImage(image_id);
	if(!ilLoadImage(filename)) return NULL;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
	bytes = ilGetData();

	pitch = width * bpp / 8;
	tmp = SDL_CreateRGBSurfaceFrom(bytes, width, height, bpp, pitch, 0x0000FF, 0xFF00, 0xFF0000, 0xFF000000);
	result = SDL_ConvertSurface(tmp, g_screen->format, SDL_SWSURFACE | SDL_SRCALPHA);
	SDL_FreeSurface(tmp);
	return result;
}

void utils_blit_with_transparency(SDL_Surface *src, SDL_Surface *dest, float x, float y)
{
	int X, Y;
	int xs, ys, ws, xd, yd, wd;
	Uint32 *srcpix, *dstpix, srcoffset, dstoffset, color;

	X = x + 0.5f;
	Y = y + 0.5f;

	if(SDL_MUSTLOCK(src)) SDL_LockSurface(src);
	if(SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
	ws = src->pitch / src->format->BytesPerPixel;
	wd = dest->pitch / dest->format->BytesPerPixel;
	srcpix = (Uint32 *)src->pixels;
	dstpix = (Uint32 *)dest->pixels;

	for(ys = 0, yd = Y; (ys < src->h); ys++, yd++)
	{
		if(yd >= dest->h) break;
		if(yd < 0) continue;
		for(xs = 0, xd = X; (xs < src->w); xs++, xd++)
		{
			if(xd >= dest->w) break;
			if(xd < 0) continue;

			srcoffset = xs + ys * ws;
			color = srcpix[srcoffset] & 0xFFFFFF;
			if(color == 0xFF00FF) continue;

			dstoffset = xd + yd * wd;
			dstpix[dstoffset] = 0xFF000000 | color;

		}
	}

	
	if(SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
	if(SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
}