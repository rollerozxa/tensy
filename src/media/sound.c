#include "sound.h"
#include "assetloader.h"
#include "gamesettings.h"

#include <data/click_ogg.h>
#include <data/match_wav.h>
#include <data/select_wav.h>
#include <data/woozy_ogg.h>

#include <SDL3_mixer/SDL_mixer.h>

extern MIX_Mixer *mixer;

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
