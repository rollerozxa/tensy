#include "color.h"
#include "consts.h"
#include "draw.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stddef.h>

static Button exit_button, back_button;

void puzzle_editor_exit_init(void) {
	BUTTON(exit_button, RECT(0, 0, 120, 35), "Exit");
	BUTTON(back_button, RECT(0, 0, 120, 35), "Back");
}

bool puzzle_editor_exit_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button)) {
		overlay_hide();
		return true;
	}

	if (button_event(ev, &exit_button)) {
		overlay_hide();
		scene_switch("mainmenu");
		return true;
	}

	return false;
}

void puzzle_editor_exit_draw(void) {
	draw_translucent_overlay();

	SDL_FRect pausebg_rect = draw_centered_fill_rect(
		POINT(20 * 15, 20 * 7.5),
		CLR_BACKGROUND_2);

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	text_draw_shadow_centered("Exit puzzle editor?", &text_rect, 2);

	text_rect.y += 40;

	text_draw_shadow_centered("Unsaved progress will be lost.", &text_rect, 1);

	exit_button.rect.x = pausebg_rect.x + pausebg_rect.w - back_button.rect.w - 10;
	exit_button.rect.y = back_button.rect.y = pausebg_rect.y + 20 * 5;
	button(&exit_button);

	back_button.rect.x = pausebg_rect.x + 10;
	button(&back_button);
}

Overlay puzzle_editor_exit_overlay = {
	"puzzle_editor_exit",
	puzzle_editor_exit_init,
	puzzle_editor_exit_event,
	NULL,
	puzzle_editor_exit_draw
};
