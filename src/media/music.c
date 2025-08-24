#include "music.h"

#include <SDL3_mixer/SDL_mixer.h>

#if SDL_MIXER_VERSION_ATLEAST(3, 1, 0)

extern MIX_Mixer *mixer;
static MIX_Audio *music_bank[100];
static MIX_Track *music_track;

#define LOAD_MUSIC(id, var) \
	music_bank[id] = MIX_LoadAudio_IO(mixer, SDL_IOFromMem(var, var##_len), false, false)

void music_init(void) {
	music_track = MIX_CreateTrack(mixer);
}

void music_play(int music_id, int loops) {
	MIX_SetTrackAudio(music_track, music_bank[music_id]);
	SDL_PropertiesID prop = SDL_CreateProperties();
	SDL_SetNumberProperty(prop, MIX_PROP_PLAY_LOOPS_NUMBER, 0);
	MIX_PlayTrack(music_track, prop);
}

void music_fade_out(int ms) {
	MIX_StopTrack(music_track, MIX_TrackMSToFrames(music_track, ms));
}

bool music_is_playing() {
	return MIX_TrackPlaying(music_track);
}

#else

void music_init(void) {}
void music_play(int music_id, int loops) {}
void music_fade_out(int ms) {}
bool music_is_playing() { return false; }

#endif
