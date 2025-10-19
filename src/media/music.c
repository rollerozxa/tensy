#include "music.h"
#include "gamesettings.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "assetloader.h"

#if EMBEDDED_DATA
static unsigned char music_credits_oxm[] = {
	#embed "data/music_credits.oxm"
};
static unsigned int music_credits_oxm_len = sizeof(music_credits_oxm);

static unsigned char music_mainmenu_xm[] = {
	#embed "data/music_mainmenu.xm"
};
static unsigned int music_mainmenu_xm_len = sizeof(music_mainmenu_xm);

#endif

extern MIX_Mixer *mixer;
static MIX_Audio *music_bank[100];
static MIX_Track *music_track;
static int last_played;

#define LOAD_MUSIC(id, data, path) \
	music_bank[id] = ASSETLOADER_MUSIC(data, path)

void music_init(void) {
	LOAD_MUSIC(MUS_CREDITS, music_credits_oxm, "music_credits.oxm");
	LOAD_MUSIC(MUS_MAINMENU, music_mainmenu_xm, "music_mainmenu.xm");

	music_track = MIX_CreateTrack(mixer);

	if (!settings_getflag(FLAG_MUSIC))
		music_mute(true);
}

void music_reset(void) {
	last_played = -1;
	music_mute(false);
}

void music_play(int music_id, int loops) {
	if (last_played == music_id && music_is_playing()) {
		return;
	}

	MIX_SetTrackAudio(music_track, music_bank[music_id]);
	SDL_PropertiesID prop = SDL_CreateProperties();
	SDL_SetNumberProperty(prop, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
	MIX_PlayTrack(music_track, prop);
	last_played = music_id;
}

void music_fade_out(int ms) {
	MIX_StopTrack(music_track, MIX_TrackMSToFrames(music_track, ms));
}

bool music_is_playing(void) {
	return MIX_TrackPlaying(music_track);
}

void music_mute(bool muted) {
	MIX_SetTrackGain(music_track, muted ? 0.0f : 1.0f);
}
