#include "consts.h"
#include "gamesettings.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "input.h"
#include "media/music.h"
#include "media/sound.h"
#include "renderer.h"
#include "scene.h"
#include "text.h"
#include "toast.h"
#include <SDL3/SDL.h>

static Checkbox mono_numbers_checkbox, sound_checkbox, reduced_motion_checkbox, fullscreen_checkbox, music_checkbox, pixel_perfect_checkbox;

static Button save_button, delete_data_button;

void settings_init(void) {
	CHECKBOX(mono_numbers_checkbox, POINT(20,80), settings_getflag(FLAG_MONO_NUMBERS), "Monochrome numbers");
	CHECKBOX(sound_checkbox, POINT(20, 80+40*1), settings_getflag(FLAG_SOUND), "Sound effects");
	CHECKBOX(reduced_motion_checkbox, POINT(20, 80+40*2), settings_getflag(FLAG_REDUCED_MOTION), "Reduced motion");
	CHECKBOX(fullscreen_checkbox, POINT(20, 80+40*3), settings_getflag(FLAG_FULLSCREEN), "Fullscreen");

	CHECKBOX(music_checkbox, POINT(320, 80), settings_getflag(FLAG_MUSIC), "Music");
	CHECKBOX(pixel_perfect_checkbox, POINT(320, 80+40*1), settings_getflag(FLAG_PIXEL_PERFECT), "Pixel perfect scaling");

	BUTTON(save_button, RECT(220,280,200,40), "Save & Go back");
	BUTTON(delete_data_button, RECT(420,20,200,40), "Clear save data");
}

extern SDL_Window *window;

void settings_event(const SDL_Event *ev) {
	if (checkbox_event(ev, &mono_numbers_checkbox))
		settings_toggleflag(FLAG_MONO_NUMBERS);

	if (checkbox_event(ev, &sound_checkbox))
		settings_toggleflag(FLAG_SOUND);

	if (checkbox_event(ev, &reduced_motion_checkbox))
		settings_toggleflag(FLAG_REDUCED_MOTION);

#ifndef ALWAYS_FULLSCREEN
	if (checkbox_event(ev, &fullscreen_checkbox)) {
		settings_toggleflag(FLAG_FULLSCREEN);
		SDL_SetWindowFullscreen(window, settings_getflag(FLAG_FULLSCREEN));
	}
#endif

	if (checkbox_event(ev, &music_checkbox)) {
		settings_toggleflag(FLAG_MUSIC);
		music_mute(!settings_getflag(FLAG_MUSIC));
	}

	if (checkbox_event(ev, &pixel_perfect_checkbox)) {
		settings_toggleflag(FLAG_PIXEL_PERFECT);
		renderer_set_logical_presentation();
	}

	if (button_event(ev, &save_button) || is_escaping(ev))
		scene_switch("mainmenu");

	if (ev->type == SDL_EVENT_KEY_UP && ev->key.scancode == SDL_SCANCODE_5 && !settings_getflag(FLAG_SECRET_FIVE)) {
		settings_toggleflag(FLAG_SECRET_FIVE);
		sound_play(SND_WOOZY);
		toast_show("...huh...?", 3);
	}

	// Ctrl+Shift+Delete to clear data
	bool special_combo = ev->type == SDL_EVENT_KEY_UP && ev->key.key == SDLK_DELETE
			&& ev->key.mod & SDL_KMOD_CTRL && ev->key.mod & SDL_KMOD_SHIFT && ev->key.mod & SDL_KMOD_ALT;

	if (button_event(ev, &delete_data_button) || special_combo) {
		scene_switch("clear_data");
	}
}

void settings_update(float dt) {
	// Update check status if user fullscreens using keybind in settings dialog
	fullscreen_checkbox.checked = settings_getflag(FLAG_FULLSCREEN);
}

void settings_draw(void) {
	text_draw_shadow("Settings", 20, 20, 3);

	checkbox(&mono_numbers_checkbox);
	checkbox(&sound_checkbox);
	checkbox(&reduced_motion_checkbox);
#ifndef ALWAYS_FULLSCREEN
	checkbox(&fullscreen_checkbox);
#endif

	checkbox(&music_checkbox);
#ifdef SUPPORTS_PIXEL_PERFECT
	checkbox(&pixel_perfect_checkbox);
#endif

	button(&save_button);
	button(&delete_data_button);
}

Scene settings_scene = {
	"settings",
	settings_init,
	settings_event,
	settings_update,
	settings_draw,
	0x1F3F8F
};
