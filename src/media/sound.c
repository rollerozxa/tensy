#include "sound.h"
#include "gamesettings.h"

#include <data/click.h>
#include <data/match.h>
#include <data/select.h>
#include <data/woozy.h>

#include <SDL3_mixer/SDL_mixer.h>

#if SDL_MIXER_VERSION_ATLEAST(3, 1, 0)
	#define SDL_REMIXER

	extern MIX_Mixer *mixer;
#else
	#define MIX_Audio Mix_Chunk
	extern int mixer;

	#define MIX_LoadAudio_IO(dummy1, src, dummy2, freesrc) Mix_LoadWAV_IO(src, freesrc)
	#define MIX_PlayAudio(dummy1, sound) Mix_PlayChannel(-1, sound, 0)
#endif

static MIX_Audio *sound_bank[100];

#define LOAD_SOUND(id, var) \
	sound_bank[id] = MIX_LoadAudio_IO(mixer, SDL_IOFromMem(var, var##_len), true, false)

void sound_init(void) {
	LOAD_SOUND(SND_CLICK, click_ogg);
	LOAD_SOUND(SND_MATCH, match_wav);
	LOAD_SOUND(SND_SELECT, select_wav);
	LOAD_SOUND(SND_WOOZY, woozy_ogg);
}

void sound_play(int sound_id) {

	if (!settings_getflag(FLAG_SOUND))
		return;

	MIX_PlayAudio(mixer, sound_bank[sound_id]);
}
