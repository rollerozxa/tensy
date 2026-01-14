#include "app.h"
#include "debug.h"
#include "gamepad.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "gui/tooltip.h"
#include "highscores.h"
#include "media/music.h"
#include "media/sound.h"
#include "media/textures.h"
#include "overlay.h"
#include "scene.h"
#include "scenes.h"
#include "screenshot.h"
#include "toast.h"
#include "virtual_cursor.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <stdint.h>

MIX_Mixer *mixer;

void AppInit(SDL_Window *window, SDL_Renderer *renderer) {
	SDL_Surface *icon = get_icon_surface();
	SDL_SetWindowIcon(window, icon);
	SDL_DestroySurface(icon);

	MIX_Init();
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, (&(SDL_AudioSpec){SDL_AUDIO_S16, 2, 44100}));

	sound_init();
	music_init();
	textures_init(renderer);

	gamepad_init();

	scenes_register();
	init_gamemodes();

	highscores_file_load();
}

void AppEvent(SDL_Event *ev) {
	gamepad_handle_hotswap(ev);
	virtual_cursor_event(ev);
	scene_run_event(ev);
	overlay_run_event(ev);

	if (ev->type == SDL_EVENT_KEY_UP && ev->key.key == SDLK_F12)
		screenshot_queue();

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
	virtual_cursor_update();
}

void AppDraw(SDL_Renderer *renderer) {
	scene_run_draw();
	overlay_run_draw();

	toast_draw();
	tooltip_draw();

	debug_draw();

	scene_perform_transition();
	virtual_cursor_draw();

	screenshot_poll();
}

void AppQuit(void) {
	settings_save();
	scene_switch("exiting");
}
