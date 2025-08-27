#include "sound.h"
#include "gamesettings.h"

#include <data/click_ogg.h>
#include <data/match_wav.h>
#include <data/select_wav.h>
#include <data/woozy_ogg.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "assetloader.h"

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

#define LOAD_SOUND(id, data, path) \
	sound_bank[id] = ASSETLOADER_SOUND(data, path)

void sound_init(void) {
	LOAD_SOUND(SND_CLICK, click_ogg, "click.ogg");
	LOAD_SOUND(SND_MATCH, match_wav, "match.wav");
	LOAD_SOUND(SND_SELECT, select_wav, "select.wav");
	LOAD_SOUND(SND_WOOZY, woozy_ogg, "woozy.ogg");
}

void sound_play(int sound_id) {

	if (!settings_getflag(FLAG_SOUND))
		return;

	MIX_PlayAudio(mixer, sound_bank[sound_id]);
}
