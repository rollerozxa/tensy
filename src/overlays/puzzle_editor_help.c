#include "color.h"
#include "consts.h"
#include "draw.h"
#include "gamestate.h"
#include "graph.h"
#include "overlay.h"
#include "text.h"

static Button back_button;

void puzzle_editor_help_init(void) {
	BUTTON(back_button, RECT(CENTER(SCREEN_W, 140), SCREEN_H - 50, 140, 40), "Back");
}

bool puzzle_editor_help_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button)) {
		overlay_hide();
		return true;
	}

	return false;
}

void puzzle_editor_help_draw(void) {
	draw_translucent_overlay();

	SDL_FRect pausebg_rect = draw_centered_fill_rect(
		POINT(20 * 26, 20 * 18),
		CLR_BACKGROUND_2);

	text_draw_shadow("Puzzle editor help", 80, 10, 3);

	const char *help_text[] = {
		"The puzzle editor is primarily keyboard-driven, you can",
		"either use arrow keys or the mouse to select a cell.",
		"",
		"Pressing a number key (1-9) will set the selected cell",
		"to that number & pressing 0 will clear the cell.",
		"",
		"Shift+arrow keys will resize the board in the given",
		"direction, and PgUp/PgDown scales the board.",
		"",
		"Have fun!",
		"                                          -ROllerozxa"
	};

	int y = 60;
	for (size_t i = 0; i < SDL_arraysize(help_text); i++) {
		const char *line = help_text[i];
		text_draw_shadow(line, 70, y, 1.5);
		y += (line[0] == '\0') ? 15 : 25;
	}

	button(&back_button);
}

Overlay puzzle_editor_help_overlay = {
	"puzzle_editor_help",
	puzzle_editor_help_init,
	puzzle_editor_help_event,
	NULL,
	puzzle_editor_help_draw
};
