#include "music.h"

#include <FMOD/fmod.h>

#include <stdio.h>

static FMOD_SYSTEM *fmod_system = NULL;
static FMOD_SOUND *fmod_music = NULL;
static FMOD_CHANNEL *fmod_channel = NULL;

void music_init(void)
{
	FMOD_RESULT result;

	result = FMOD_System_Create(&fmod_system);
	if(result != FMOD_OK)
	{
		printf("Error at creation of FMOD Ex system\n");
		return;
	}

	result = FMOD_System_Init(fmod_system, 1, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		printf("Error at init of FMOD Ex system\n");
		return;
	}
}

void music_start(char *filename)
{
	FMOD_RESULT result;

	result = FMOD_System_CreateSound(fmod_system, filename, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &fmod_music);
	if(result != FMOD_OK)
	{
		printf("Error at loading music %s\n", filename);
		return;
	}

	result = FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmod_music, 0, &fmod_channel);
	if(result != FMOD_OK)
	{
		printf("Error at playing music\n");
		return;
	}
}

void music_stop(void)
{
	FMOD_Channel_SetPaused(fmod_channel, 1);
	FMOD_Sound_Release(fmod_music);
}

void music_deinit(void)
{
	FMOD_System_Close(fmod_system);
	FMOD_System_Release(fmod_system);
}