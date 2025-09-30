#include "consts.h"
#include "draw.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "gamestate.h"
#include "gui/button.h"
#include "gui/textinput.h"
#include "highscores.h"
#include "input.h"
#include "overlay.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static Button exit_button;
static TextInput name_input;

void gameover_init(void) {
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Return");
	TEXTINPUT(name_input, RECT(0, 0, 200, 40), "Player");

	textinput_settext(&name_input, settings()->last_username);
}

void gameover_event(const SDL_Event *ev) {
	if (button_event(ev, &exit_button) || is_escaping(ev)) {
		highscore_register(game, name_input.buffer);
		strncpy(settings()->last_username, name_input.buffer, 12);
		overlay_hide();
		scene_switch("selectmode");
	}

	textinput_event(ev, &name_input);
}

void gameover_draw(void) {
	draw_translucent_overlay();

	SDL_FRect bg_rect = draw_centered_fill_rect(POINT(20 * 10, 20 * 12));

	SDL_FRect text_rect = {bg_rect.x, bg_rect.y, bg_rect.w, 20*2};
	if (current_gamemode().time_limit)
		text_draw_shadow_centered("Out of time!", &text_rect, 2);
	else
		text_draw_shadow_centered("Game Over!", &text_rect, 2);

	SDL_FPoint score_pos = { bg_rect.x + 20, bg_rect.y + 20*2.5 };

	text_draw_shadow("Final score:", score_pos.x, score_pos.y, 1.5);
	FMT_STRING(scoretext, 128, "%d", game.score);
	score_pos.y += 25;
	text_draw_shadow(scoretext, score_pos.x, score_pos.y, 1.5);

	score_pos.y += 30;
	text_draw_shadow("Your name:", score_pos.x, score_pos.y, 1.5);

	name_input.rect.x = bg_rect.x + 20;
	name_input.rect.y = score_pos.y + 20;
	name_input.rect.w = bg_rect.w - 40;
	textinput(&name_input);

	exit_button.rect.x = bg_rect.x + CENTER(bg_rect.w, exit_button.rect.w);
	exit_button.rect.y = bg_rect.y + 20*9;

	button(&exit_button);
}

Overlay gameover_overlay = {
	"gameover",
	gameover_init,
	gameover_event,
	NULL,
	gameover_draw
};
