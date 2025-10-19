#include "app.h"
#include "debug.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "media/music.h"
#include "media/sound.h"
#include "media/textures.h"
#include "overlay.h"
#include "scene.h"
#include "scenes.h"
#include "toast.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <stdint.h>

MIX_Mixer *mixer;

void AppInit(SDL_Window *window, SDL_Renderer *renderer) {
	MIX_Init();
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, (&(SDL_AudioSpec){SDL_AUDIO_S16, 2, 44100}));

	sound_init();
	music_init();
	textures_init(renderer);

	SDL_Surface *icon = get_icon_surface();
	SDL_SetWindowIcon(window, icon);
	SDL_DestroySurface(icon);

	scenes_register();
	init_gamemodes();
}

void AppEvent(SDL_Event *ev) {
	scene_run_event(ev);
	overlay_run_event(ev);

	debug_event(ev);
}

void AppUpdate(void) {
	static uint64_t last_time = 0;
	if (last_time == 0)
		last_time = SDL_GetTicksNS();

	uint64_t now = SDL_GetTicksNS();
	float dt = (now - last_time) / 1e9f;
	last_time = now;

	settings_savetimer(dt);

	scene_run_update(dt);
	overlay_run_update();

	toast_update(dt);
}

void AppDraw(SDL_Renderer *renderer) {
	scene_run_draw();
	overlay_run_draw();

	toast_draw();

	debug_draw();

	scene_perform_transition();
}

void AppQuit(void) {
	settings_save();
	scene_switch("exiting");
}
