#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

void music_init(void);
void music_play(int music_id, int loops);
void music_fade_out(int ms);
bool music_is_playing();
