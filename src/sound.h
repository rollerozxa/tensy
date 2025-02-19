#pragma once

#include <SDL3/SDL.h>

enum {
	SND_CLICK
};

void sound_init(void);
void sound_play(int sound_id);
