#pragma once

#include <SDL3/SDL.h>

enum {
	SND_CLICK,
	SND_MATCH,
	SND_SELECT
};

void sound_init(void);
void sound_play(int sound_id);
