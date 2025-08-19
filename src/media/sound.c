#include "sound.h"
#include "gamesettings.h"

#include <data/click.h>
#include <data/match.h>
#include <data/select.h>
#include <data/woozy.h>

#include <SDL3_mixer/SDL_mixer.h>

#if SDL_MIXER_VERSION_ATLEAST(3, 1, 0)
	#define SDL_REMIXER

	static MIX_Mixer *mixer;
#else
	#define MIX_Audio Mix_Chunk
	static int mixer;

	#define MIX_Init()
	#define MIX_LoadAudio_IO(dummy1, src, dummy2, freesrc) Mix_LoadWAV_IO(src, freesrc)
	#define MIX_CreateMixerDevice(dummy1, audiospec) Mix_OpenAudio(0, audiospec)
	#define MIX_PlayAudio(dummy1, sound) Mix_PlayChannel(-1, sound, 0)
#endif

static MIX_Audio *sound_bank[100];
static bool sound_loaded = false;

#define LOAD_SOUND(id, var) \
	sound_bank[id] = MIX_LoadAudio_IO(mixer, SDL_IOFromMem(var, var##_len), true, false)

void sound_init(void) {
	MIX_Init();
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, (&(SDL_AudioSpec){SDL_AUDIO_S16, 2, 44100}));

	LOAD_SOUND(SND_CLICK, click_ogg);
	LOAD_SOUND(SND_MATCH, match_wav);
	LOAD_SOUND(SND_SELECT, select_wav);
	LOAD_SOUND(SND_WOOZY, woozy_ogg);

	sound_loaded = true;
}

void sound_play(int sound_id) {
	if (!sound_loaded)
		sound_init();

	if (!settings_getflag(FLAG_SOUND))
		return;

	MIX_PlayAudio(mixer, sound_bank[sound_id]);
}
