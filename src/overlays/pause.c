#include "pause.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "overlay.h"
#include "render.h"
#include "scene.h"

static Button resume_button, exit_button;

void pause_init(void) {
	BUTTON(resume_button, RECT(0, 0, 150, 40), "Resume");
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Exit");
}

void pause_event(const SDL_Event *ev) {
	if (button_event(ev, &resume_button))
		hide_overlay();

	if (button_event(ev, &exit_button)) {
		hide_overlay();
		switch_scene("mainmenu");
	}
}

void pause_draw(SDL_Renderer *renderer) {
	draw_translucent_overlay(renderer);

	SDL_FRect pausebg_rect = draw_centered_fill_rect(renderer, POINT(20 * 10, 20 * 12));

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	draw_text_shadow_centered(renderer, "Game paused", &text_rect, 2);

	resume_button.rect.x = pausebg_rect.x + CENTER(pausebg_rect.w, resume_button.rect.w);
	resume_button.rect.y = pausebg_rect.y + 20*4;

	button(renderer, &resume_button);

	exit_button.rect.x = pausebg_rect.x + CENTER(pausebg_rect.w, exit_button.rect.w);
	exit_button.rect.y = pausebg_rect.y + 20*9;

	button(renderer, &exit_button);
}

Overlay pause_overlay = {
	"pause",
	pause_init,
	pause_event,
	NULL,
	pause_draw
};
