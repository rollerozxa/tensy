#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gamesettings.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "highscores.h"
#include "input.h"
#include "media/music.h"
#include "savestate.h"
#include "scene.h"
#include "text.h"

static Button confirm_button, cancel_button;
static Checkbox confirm_checkbox;

void clear_data_init(void) {
	BUTTON(confirm_button, RECT(0, 0, 120, 35), "Confirm");
	confirm_button._disabled = true;
	BUTTON(cancel_button, RECT(0, 0, 120, 35), "Cancel");
	CHECKBOX(confirm_checkbox, POINT(225, 200), false, "Yes, I'm sure");
}

void clear_data_event(const SDL_Event *ev) {
	if (confirm_checkbox.checked && button_event(ev, &confirm_button)) {
		settings_clear();
		highscores_clear();
		savestate_delete();
		music_reset();
		scene_switch_instant("intro");
	}

	if (checkbox_event(ev, &confirm_checkbox))
		confirm_button._disabled = !confirm_checkbox.checked;

	if (button_event(ev, &cancel_button) || is_escaping(ev))
		scene_switch("settings");
}

void clear_data_update(float dt) {

}

void clear_data_draw(void) {
	SDL_FRect bg_rect = draw_centered_fill_rect(POINT(20 * 24, 20 * 12), 0x632a10);

	font_set_color(CLR_WHITE);
	SDL_FRect text_rect = {bg_rect.x, bg_rect.y + 20, bg_rect.w, 20 * 4};
	text_draw_shadow_centered("Are you sure you want to clear data?", &text_rect, 2);
	text_rect.y += 40;
	text_draw_shadow_centered("This action cannot be undone.", &text_rect, 2);

	confirm_button.rect.x = bg_rect.x + bg_rect.w - confirm_button.rect.w - 10;
	confirm_button.rect.y = cancel_button.rect.y = bg_rect.y + bg_rect.h - 45;
	button(&confirm_button);

	cancel_button.rect.x = bg_rect.x + 10;
	button(&cancel_button);

	checkbox(&confirm_checkbox);
}

Scene clear_data_scene = {
	"clear_data",
	clear_data_init,
	clear_data_event,
	clear_data_update,
	clear_data_draw,
	0x8F3F1F
};
