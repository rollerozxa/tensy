#include "music.h"
#include "gamesettings.h"

#include <data/music_credits_oxm.h>
#include <data/music_mainmenu_xm.h>

#include <SDL3_mixer/SDL_mixer.h>

#if SDL_MIXER_VERSION_ATLEAST(3, 1, 0)

extern MIX_Mixer *mixer;
static MIX_Audio *music_bank[100];
static MIX_Track *music_track;
static int last_played;

#define LOAD_MUSIC(id, var) \
	music_bank[id] = MIX_LoadAudio_IO(mixer, SDL_IOFromMem(var, var##_len), false, false)

void music_init(void) {
	LOAD_MUSIC(MUS_CREDITS, music_credits_oxm);
	LOAD_MUSIC(MUS_MAINMENU, music_mainmenu_xm);

	music_track = MIX_CreateTrack(mixer);

	if (!settings_getflag(FLAG_MUSIC))
		music_mute(true);
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

bool music_is_playing() {
	return MIX_TrackPlaying(music_track);
}

void music_mute(bool muted) {
	MIX_SetTrackGain(music_track, muted ? 0.0f : 1.0f);
}

#else
void music_init(void) {}
void music_play(int music_id, int loops) {}
void music_fade_out(int ms) {}
bool music_is_playing() { return false; }
void music_mute(bool muted) {}
#endif
