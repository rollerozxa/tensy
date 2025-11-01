#include "sound.h"
#include "gamesettings.h"
#include <SDL3_mixer/SDL_mixer.h>

#include "assetloader.h"

#if EMBEDDED_DATA
#include <data/click_ogg.h>
#include <data/fall_wav.h>
#include <data/match_wav.h>
#include <data/select_wav.h>
#include <data/shuffle_ogg.h>
#include <data/woozy_ogg.h>
#endif

extern MIX_Mixer *mixer;

static MIX_Audio *sound_bank[100];

#define LOAD_SOUND(id, data, path) \
	sound_bank[id] = ASSETLOADER_SOUND(data, path)

void sound_init(void) {
	LOAD_SOUND(SND_CLICK, click_ogg, "click.ogg");
	LOAD_SOUND(SND_FALL, fall_wav, "fall.wav");
	LOAD_SOUND(SND_MATCH, match_wav, "match.wav");
	LOAD_SOUND(SND_SELECT, select_wav, "select.wav");
	LOAD_SOUND(SND_SHUFFLE, shuffle_ogg, "shuffle.ogg");
	LOAD_SOUND(SND_WOOZY, woozy_ogg, "woozy.ogg");
}

void sound_play(int sound_id) {

	if (!settings_getflag(FLAG_SOUND))
		return;

	MIX_PlayAudio(mixer, sound_bank[sound_id]);
}
