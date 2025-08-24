#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

enum {
	SND_CLICK,
	SND_MATCH,
	SND_SELECT,
	SND_WOOZY
};

void sound_init(void);
void sound_play(int sound_id);
