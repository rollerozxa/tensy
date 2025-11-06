#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "media/sound.h"
#include "mouse.h"
#include "savestate.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>

static int selected_mode = -1;
static float xoff, xvel, motion = 0;
static bool holdingdown = false;
static bool has_savestate = false;

static Button continue_button;

float lerp(float a, float b, float t) {
	return a * (1-t) + b * t;
}

void selectmode_init(void) {
	BUTTON(continue_button, RECT(SCREEN_W-180, 0, 180, 40), "Continue save");

	selected_mode = -1;
	has_savestate = savestate_exists();
}

static void select_mode_num(int num) {
	if (num >= gamemode_count || (gamemodes[num].sm_enabled && !gamemodes[num].sm_enabled()))
		return;

	sound_play(SND_CLICK);
	gamemodes[num].sm_click();
}

void selectmode_event(const SDL_Event *ev) {
	switch (ev->type) {
		case SDL_EVENT_MOUSE_WHEEL:
			xvel -= ev->wheel.y * 45;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			holdingdown = true;
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (xvel < 0.05 && selected_mode != -1 && !scene_is_transitioning()) {
				sound_play(SND_CLICK);
				gamemodes[gamemode_selectmode[selected_mode]].sm_click();
			}
			motion = 0;
			holdingdown = false;
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (holdingdown) {
				xvel -= ev->motion.xrel;
				motion += SDL_fabs(ev->motion.xrel);
			}
			break;

		case SDL_EVENT_KEY_UP:
			if (is_escaping(ev))
				scene_switch("mainmenu");

			if (ev->key.key == SDLK_1) select_mode_num(0);
			if (ev->key.key == SDLK_2) select_mode_num(1);
			if (ev->key.key == SDLK_3) select_mode_num(2);
			if (ev->key.key == SDLK_4) select_mode_num(3);
			if (ev->key.key == SDLK_5) select_mode_num(4);
			if (ev->key.key == SDLK_6) select_mode_num(5);
			if (ev->key.key == SDLK_7) select_mode_num(6);
			if (ev->key.key == SDLK_8) select_mode_num(7);
			if (ev->key.key == SDLK_9) select_mode_num(8);
			if (ev->key.key == SDLK_0) select_mode_num(0);

			break;
	}

	if (savestate_exists() && button_event(ev, &continue_button)) {
		savestate_load();
		game.loaded_existing = true;
		scene_switch("game");
	}
}

void selectmode_update(float dt) {
	xoff += lerp(xvel, 0, 0.75);
	xvel -= lerp(xvel, 0, 0.75);

	if (xoff < 0) {
		xoff = lerp(xoff, 0, 0.5);
	}
}

void selectmode_draw(void) {

	text_draw_shadow("Select mode", 10, 10, 3);

	for (size_t i = 0; i < gamemode_count; i++) {
		font_set_color(CLR_WHITE);

		GameMode mode = gamemodes[gamemode_selectmode[i]];

		if (mode.sm_enabled != NULL && !mode.sm_enabled())
			continue;

		SDL_FRect rect = {
			10 + i * 240 - xoff, 54, 230, 280
		};

		int mod = 20;
		SDL_FPoint mouse;
		int pressed = mouse_get_state_scaled(&mouse);
		if (SDL_PointInRectFloat(&mouse, &rect) && !mode.disabled) {
			if (motion < 4) {
				if (pressed) {
					selected_mode = i;
					mod = -10;
				} else
					mod = 30;
			} else
				selected_mode = -1;
		}
		SDL_Color color = {31+mod, 63+mod, 143+mod};
		draw_set_color(color_sdl_to_bitpack(color));

		draw_fill_rect(&rect);

		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 4; y++) {
				if (mode.board[y][x] == 0) continue;

				SDL_FPoint point = {
					x * 30 + rect.x + 28,
					y * 30 + rect.y + 10};

				if (!settings_getflag(FLAG_MONO_NUMBERS))
					font_set_color(color_numbers(mode.board[y][x]));

				font_draw_char_shadow(mode.board[y][x] + '0', point.x, point.y, 3);
			}
		}

		font_set_color(CLR_WHITE);

		if (mode.sm_draw)
			mode.sm_draw(rect);

		float centerx = (rect.w - text_calc_rect(mode.name, 3).w) / 2;
		text_draw_shadow(mode.name, rect.x + centerx, 205, 3);

		for (int j = 0; j < 4; j++) {
			if (mode.description[j])
				text_draw_shadow(mode.description[j], rect.x + 6, 240 + j * 20, 2);
		}

		if (mode.disabled) {
			draw_set_color_alpha(0x000000A0);
			draw_fill_rect(&rect);
			const char *coming_soon = "Coming soon!";
			font_set_color((SDL_Color){0xFF, 0x00, 0x00, 0xFF});
			for (size_t i = 0; coming_soon[i] != '\0'; i++) {
				font_draw_char_shadow(coming_soon[i], rect.x + i * GLYPH_WIDTH * 3 + 5, rect.y + i * 10 + 5, 3);
			}
		}
	}

	if (has_savestate)
		button(&continue_button);
}

Scene selectmode_scene = {
	"selectmode",
	selectmode_init,
	selectmode_event,
	selectmode_update,
	selectmode_draw,
	0x1F3F8F
};
