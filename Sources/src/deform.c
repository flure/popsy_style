#include "deform.h"

#include <math.h>
#include "SDL.h"

#include "utils.h"

void deform_draw_with_lut(unsigned current_time, SDL_Surface *src, SDL_Surface *dst, int *lut)
{
	int x, y;
	int w = src->w, h = src->h;
	unsigned u, v;
	unsigned offset, lut_offset, offset_uv;
	unsigned *src_pixels, *dst_pixels;
	unsigned width = dst->pitch / dst->format->BytesPerPixel;

	if(SDL_MUSTLOCK(dst)) SDL_LockSurface(dst);
	src_pixels = (unsigned *)src->pixels;
	dst_pixels = (unsigned *)dst->pixels;
	offset = 0;
	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			lut_offset = ((x&511) + ((y&511)*512)) * 2;
			u = (lut[lut_offset + 0] + current_time);
			v = (lut[lut_offset + 1] + current_time);
			offset_uv = ((x + u)%w) + ((y + v)%h)*w;
			offset = x + y*width;
			dst_pixels[offset] = src_pixels[offset_uv];
			offset++;
		}
	}
	if(SDL_MUSTLOCK(dst)) SDL_UnlockSurface(dst);
}

void deform_draw_with_lut_2(unsigned current_time, SDL_Surface *src, SDL_Surface *dst, int *lut1, int *lut2)
{
	int x, y;
	int w = src->w, h = src->h;
	unsigned offset, u, v, lut_offset, offset_uv;
	unsigned *src_pixels, *dst_pixels;
	unsigned width = dst->pitch / dst->format->BytesPerPixel;

	if(SDL_MUSTLOCK(dst)) SDL_LockSurface(dst);
	src_pixels = (unsigned *)src->pixels;
	dst_pixels = (unsigned *)dst->pixels;
	offset = 0;
	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			lut_offset = ((x&511) + ((y&511)*512)) * 2;
			u = (lut1[lut_offset + 0] + lut2[lut_offset + 0] + current_time);
			v = (lut1[lut_offset + 1] + lut2[lut_offset + 1] + current_time);
			offset_uv = ((x + u)%w) + ((y + v)%h)*w;
			offset = x + y*width;
			dst_pixels[offset] = src_pixels[offset_uv];
			offset++;
		}
	}
	if(SDL_MUSTLOCK(dst)) SDL_UnlockSurface(dst);
}

void deform_lut_identity(int *lut)
{
	int i;
	for(i = 0; i < 512*512; i++)
	{
		lut[i * 2 + 0] = 0;
		lut[i * 2 + 1] = 0;
	}
}

void deform_lut_a(int *lut)
{
	int x, y;
	float fx, fy, d, a;
	float u, v;
	for(y = 0; y < 512; y++)
	{
		for(x = 0; x < 512; x++)
		{
			fx = -1.00f + 2.00f*(float)x/320.0f;
        	fy = -1.00f + 2.00f*(float)y/240.0f;
        	d = sqrtf( fx*fx + fy*fy );
        	a = atan2f( fy, fx );

        	u = cosf( a )/d;
        	v = sinf( a )/d;
        	lut[(x + y*512)*2 + 0] = (int)(u * 256.0f);//64.0f);
        	lut[(x + y*512)*2 + 1] = (int)(v * 64.0f);//64.0f);
		}
	}
}

void deform_lut_b(int *lut)
{
	int x, y;
	float fx, fy, d, a;
	float u, v;
	for(y = 0; y < 512; y++)
	{
		for(x = 0; x < 512; x++)
		{
			fx = -1.00f + 2.00f*(float)x/320.0f;
        	fy = -1.00f + 2.00f*(float)y/240.0f;
        	d = sqrtf( fx*fx + fy*fy );
        	a = atan2f( fy, fx );
        	u = d * cosf(a + d);
        	v = d * sinf(a + d);
        	lut[(x + y*512)*2 + 0] = (int)(u * 64.0f);
        	lut[(x + y*512)*2 + 1] = (int)(v * 64.0f);
		}
	}	
}


void deform_lut_c(int *lut)
{
	int x, y;
	float fx, fy, d;
	float u, v;
	for(y = 0; y < 512; y++)
	{
		for(x = 0; x < 512; x++)
		{
			fx = -1.00f + 2.00f*(float)x/320.0f;
        	fy = -1.00f + 2.00f*(float)y/240.0f;
        	d = sqrtf( fx*fx + fy*fy );
        	// a = atan2f( fy, fx );
        	u = 0.5f * d / _M_PI;
        	v = sinf(7.0f * d);
        	lut[(x + y*512)*2 + 0] = (int)(u * 64.0f);
        	lut[(x + y*512)*2 + 1] = (int)(v * 64.0f);
		}
	}	
}

void deform_lut_d(int *lut)
{
	int x, y;
	float fx, fy, d;
	float u, v;
	for(y = 0; y < 512; y++)
	{
		for(x = 0; x < 512; x++)
		{
			fx = -1.00f + 2.00f*(float)x/320.0f;
        	fy = -1.00f + 2.00f*(float)y/240.0f;
        	d = sqrtf( fx*fx + fy*fy );
        	// a = atan2f( fy, fx );
        	u = 0.1f * fx / (0.11f + d * 0.5f);
        	v = 0.1f * fy / (0.11f + d * 0.5f);
        	lut[(x + y*512)*2 + 0] = (int)(u * 256.0f);
        	lut[(x + y*512)*2 + 1] = (int)(v * 256.0f);
		}
	}	
}