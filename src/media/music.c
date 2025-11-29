#include "music.h"
#include "gamesettings.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "assetloader.h"

#if EMBEDDED_DATA
#include <data/music_credits_oxm.h>
#include <data/music_mainmenu_xm.h>
#include <data/music_mystery_ogg.h>
#endif

extern MIX_Mixer *mixer;
static MIX_Audio *music_bank[100];
static MIX_Track *music_track;
static int last_played;
static bool music_muted = false;

#define LOAD_MUSIC(id, data, path) \
	music_bank[id] = ASSETLOADER_MUSIC(data, path)

void music_init(void) {
	LOAD_MUSIC(MUS_CREDITS, music_credits_oxm, "music_credits.oxm");
	LOAD_MUSIC(MUS_MAINMENU, music_mainmenu_xm, "music_mainmenu.xm");
	LOAD_MUSIC(MUS_MYSTERY, music_mystery_ogg, "music_mystery.ogg");

	music_track = MIX_CreateTrack(mixer);

	if (!settings_getflag(FLAG_MUSIC))
		music_mute(true);
}

void music_reset(void) {
	last_played = -1;
	music_mute(false);
}

static void music_playtrack(void) {
	SDL_PropertiesID prop = SDL_CreateProperties();
	SDL_SetNumberProperty(prop, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
	MIX_PlayTrack(music_track, prop);
	SDL_DestroyProperties(prop);
}

void music_play(int music_id, int loops) {
	if (last_played == music_id && music_is_playing())
		return;

	last_played = music_id;
	MIX_SetTrackAudio(music_track, music_bank[music_id]);

	if (music_muted)
		return;

	music_playtrack();
}

void music_fade_out(int ms) {
	MIX_StopTrack(music_track, MIX_TrackMSToFrames(music_track, ms));
}

bool music_is_playing(void) {
	return MIX_TrackPlaying(music_track);
}

void music_mute(bool muted) {
	if (music_muted == muted)
		return;

	music_muted = muted;

	if (muted)
		music_fade_out(0);
	else
		music_playtrack();
}
