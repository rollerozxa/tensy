#include "settings.h"
#include "consts.h"
#include "gamesettings.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "input.h"
#include "media/sound.h"
#include "scene.h"
#include "text.h"
#include "toast.h"

static Checkbox colored_numbers_checkbox;

static Button save_button;

void settings_init(void) {
	CHECKBOX(colored_numbers_checkbox, POINT(20,80), false, "Coloured numbers");
	BUTTON(save_button, RECT(220,280,200,40), "Save & Go back");

	colored_numbers_checkbox.checked = settings_getflag(FLAG_COLORED_NUMBERS);
}

void settings_event(const SDL_Event *ev) {
	if (checkbox_event(ev, &colored_numbers_checkbox))
		settings_toggleflag(FLAG_COLORED_NUMBERS);

	if (button_event(ev, &save_button) || is_escaping(ev))
		scene_switch("mainmenu");

	if (ev->type == SDL_EVENT_KEY_UP && ev->key.scancode == SDL_SCANCODE_5 && !settings_getflag(FLAG_SECRET_FIVE)) {
		settings_toggleflag(FLAG_SECRET_FIVE);
		sound_play(SND_WOOZY);
		toast_show("...huh...?", 3);
	}
}

void settings_draw(SDL_Renderer *renderer) {
	text_draw_shadow("Settings", 20, 20, 3);

	checkbox(&colored_numbers_checkbox);
	button(&save_button);
}

Scene settings_scene = {
	"settings",
	settings_init,
	settings_event,
	NULL,
	settings_draw,
	0x1F3F8F
};
