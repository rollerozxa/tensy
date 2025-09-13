#pragma once

#include <stdbool.h>

enum {
	MUS_CREDITS,
	MUS_MAINMENU
};

void music_init(void);
void music_play(int music_id, int loops);
void music_fade_out(int ms);
bool music_is_playing(void);
void music_mute(bool muted);
