#include "settings.h"
#include "consts.h"
#include "colours.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "font.h"
#include "render.h"
#include "scene.h"

// Settings storage

static Settings settings_data = {
	false,
	false,
};

Settings *settings(void) {
	return &settings_data;
}

// Settings scene

static Checkbox board_physics_checkbox, coloured_numbers_checkbox;

static Button save_button;

void settings_init(void) {
	CHECKBOX(board_physics_checkbox, POINT(20,80), false, "Board physics");
	CHECKBOX(coloured_numbers_checkbox, POINT(20,120), false, "Coloured numbers");
	BUTTON(save_button, RECT(220,280,200,40), "Save & Go back");
}

void settings_event(const SDL_Event *ev) {
	if (checkbox_event(ev, &board_physics_checkbox))
		settings()->board_physics = board_physics_checkbox.checked;

	if (checkbox_event(ev, &coloured_numbers_checkbox))
		settings()->coloured_numbers = coloured_numbers_checkbox.checked;

	if (button_event(ev, &save_button))
		switch_scene("mainmenu");
}

void settings_update(void) {

}

void settings_draw(SDL_Renderer *renderer) {
	set_font_color(CLR_WHITE);
	draw_text_shadow(renderer, "Settings", 20, 20, 3);

	checkbox(renderer, &board_physics_checkbox);
	checkbox(renderer, &coloured_numbers_checkbox);
	button(renderer, &save_button);
}

Scene settings_scene = {
	"settings",
	settings_init,
	settings_event,
	settings_update,
	settings_draw,
	0x1F3F8F
};
