#include "sound.h"
#include "gamesettings.h"
#include <SDL3_mixer/SDL_mixer.h>

#include "assetloader.h"

#if EMBEDDED_DATA
static unsigned char click_ogg[] = {
	#embed "data/click.ogg"
};
static unsigned int click_ogg_len = sizeof(click_ogg);

static unsigned char match_wav[] = {
	#embed "data/match.wav"
};
static unsigned int match_wav_len = sizeof(match_wav);

static unsigned char select_wav[] = {
	#embed "data/select.wav"
};
static unsigned int select_wav_len = sizeof(select_wav);

static unsigned char woozy_ogg[] = {
	#embed "data/woozy.ogg"
};
static unsigned int woozy_ogg_len = sizeof(woozy_ogg);

#endif

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
