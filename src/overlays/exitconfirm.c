#include "exitconfirm.h"
#include "consts.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "render.h"
#include "scene.h"
#include "text.h"

static Button exit_button, back_button;

void exitconfirm_init(void) {
	BUTTON(exit_button, RECT(0, 0, 120, 35), "Exit");
	BUTTON(back_button, RECT(0, 0, 120, 35), "Back");
}

void exitconfirm_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button) || is_escaping(ev))
		switch_overlay("pause");

	if (button_event(ev, &exit_button)) {
		hide_overlay();
		switch_scene("mainmenu");
	}
}

void exitconfirm_draw(SDL_Renderer *renderer) {
	draw_translucent_overlay(renderer);

	SDL_FRect pausebg_rect = draw_centered_fill_rect(renderer, POINT(20 * 15, 20 * 7.5));

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	text_draw_shadow_centered(renderer, "Really exit?", &text_rect, 2);

	text_rect.y += 40;

	text_draw_shadow_centered(renderer, "Unsaved progress will be lost.", &text_rect, 1);

	exit_button.rect.x = pausebg_rect.x + pausebg_rect.w - back_button.rect.w - 10;
	exit_button.rect.y = back_button.rect.y = pausebg_rect.y + 20 * 5;
	button(renderer, &exit_button);

	back_button.rect.x = pausebg_rect.x + 10;
	button(renderer, &back_button);
}

Overlay exitconfirm_overlay = {
	"exitconfirm",
	exitconfirm_init,
	exitconfirm_event,
	NULL,
	exitconfirm_draw
};
