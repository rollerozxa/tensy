#include "color.h"
#include "consts.h"
#include "draw.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "scene.h"
#include "text.h"

static Button ok_button;

void puzzle_note_init(void) {
	BUTTON(ok_button, RECT(CENTER(SCREEN_W, 140), SCREEN_H - 50, 140, 40), "Okay");
}

bool puzzle_note_event(const SDL_Event *ev) {
	if (is_escaping(ev)) {
		overlay_hide();
		return true;
	}

	if (button_event(ev, &ok_button)) {
		overlay_hide();
		scene_switch("puzzle_editor");
		return true;
	}

	return false;
}

void puzzle_note_draw(void) {
	draw_translucent_overlay();

	SDL_FRect pausebg_rect = draw_centered_fill_rect(
		POINT(20 * 26, 20 * 18),
		CLR_BACKGROUND_2);

	text_draw_shadow("Note!", 80, 10, 3);

	const char *help_text[] = {
		"The puzzle game mode is new in Tensy 1.1 and an ",
		"experimental feature. Currently there is a level editor",
		"allowing you to load and play puzzle levels, as well as ",
		"creating your own and saving them to file.",
		"",
		"The level editor itself only works with a keyboard,",
		"see the help dialog for more info."
	};

	int y = 60;
	for (size_t i = 0; i < SDL_arraysize(help_text); i++) {
		const char *line = help_text[i];
		text_draw_shadow(line, 70, y, 1.5);
		y += (line[0] == '\0') ? 15 : 25;
	}

	button(&ok_button);
}

Overlay puzzle_note_overlay = {
	"puzzle_note",
	puzzle_note_init,
	puzzle_note_event,
	NULL,
	puzzle_note_draw
};
