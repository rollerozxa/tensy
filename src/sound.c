#include "sound.h"

#include <data/click.h>
#include <data/match.h>
#include <data/select.h>

#include <SDL3_mixer/SDL_mixer.h>

static Mix_Chunk *sound_bank[100];
static bool sound_loaded = false;

#define LOAD_SOUND(id, var) \
	sound_bank[id] = Mix_LoadWAV_IO(SDL_IOFromMem(var, var##_len), true)

void sound_init(void) {
	Mix_OpenAudio(0, &(SDL_AudioSpec){SDL_AUDIO_S16, 2, 44100});

	LOAD_SOUND(SND_CLICK, click_ogg);
	LOAD_SOUND(SND_MATCH, match_wav);
	LOAD_SOUND(SND_SELECT, select_wav);

	sound_loaded = true;
}

void sound_play(int sound_id) {
	if (!sound_loaded)
		sound_init();

	Mix_PlayChannel(-1, sound_bank[sound_id], 0);
}
