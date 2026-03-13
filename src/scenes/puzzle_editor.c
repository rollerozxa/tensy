#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "input.h"
#include "media/sound.h"
#include "mouse.h"
#include "scene.h"
#include "text.h"
#include "toast.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_W 32
#define MAX_H 32

static SDL_Point dim = {10, 8};
static int board[MAX_H][MAX_W];
static SDL_Point sel = {0, 0};

static void puzzle_editor_init(void) {
	for (int y = 0; y < MAX_H; y++)
		for (int x = 0; x < MAX_W; x++)
			board[y][x] = 0;

	dim = (SDL_Point){10, 8};
	sel = (SDL_Point){0, 0};
}

static void copy_board_to_clipboard(void) {
	char *buf = malloc(2048);
	char *p = buf;
	int n = snprintf(p, 2048, "%d\n%d\n30\n", dim.x, dim.y);
	p += n;

	for (int y = 0; y < dim.y; y++) {
		for (int x = 0; x < dim.x; x++) {
			int v = board[y][x];
			*p++ = (char)('0' + v);
		}
		*p++ = '\n';
	}
	*p = '\0';

	SDL_SetClipboardText(buf);
	free(buf);

	sound_play(SND_WOOZY);
	toast_show("Board copied to clipboard", 3);
}

static void copy_board_from_clipboard(void) {
	// TODO
}

#define CELL 30.0f

static void puzzle_editor_event(const SDL_Event *ev) {
	if (is_escaping(ev)) {
		scene_switch("selectmode");
		return;
	}

	switch (ev->type) {

	case SDL_EVENT_MOUSE_BUTTON_DOWN: {}
		SDL_FPoint mouse;
		mouse_get_state_scaled(&mouse);

		// center grid
		float grid_w = dim.x * CELL;
		float grid_h = dim.y * CELL;
		float gx = (SCREEN_W - grid_w) / 2.0f;
		float gy = (SCREEN_H - grid_h) / 2.0f;
		if (SDL_PointInRectFloat(&POINT(mouse.x, mouse.y), &RECT(gx, gy, grid_w, grid_h))) {
			int cx = (int)((mouse.x - gx) / CELL);
			int cy = (int)((mouse.y - gy) / CELL);
			if (cx >= 0 && cx < dim.x && cy >= 0 && cy < dim.y)
				sel = (SDL_Point){cx, cy};
		}
		break;

	case SDL_EVENT_KEY_DOWN: {}
		SDL_Keycode key = ev->key.key;
		SDL_Keymod mods = SDL_GetModState();

		// Clipboard copy: Ctrl+C
		if ((mods & SDL_KMOD_CTRL) && (key == SDLK_C))
			copy_board_to_clipboard();

		// Ctrl+V: paste from clipboard is not implemented
		if ((mods & SDL_KMOD_CTRL) && (key == SDLK_V))
			copy_board_from_clipboard();

		// Page up/down with SHIFT change size by 0.5
		if (key == SDLK_PAGEUP) {
			// TODO
		}
		if (key == SDLK_PAGEDOWN) {
			// TODO
		}

		// Arrow keys
		if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT || key == SDLK_RIGHT) {
			if (mods & SDL_KMOD_SHIFT) {
				// resize integer steps
				if (key == SDLK_LEFT)  dim.x--;
				if (key == SDLK_RIGHT) dim.x++;
				if (key == SDLK_UP)    dim.y--;
				if (key == SDLK_DOWN)  dim.y++;

				dim.x = SDL_clamp(dim.x, 1, MAX_W);
				dim.y = SDL_clamp(dim.y, 1, MAX_H);
			} else {
				// move selector
				if (key == SDLK_LEFT)  sel.x--;
				if (key == SDLK_RIGHT) sel.x++;
				if (key == SDLK_UP)    sel.y--;
				if (key == SDLK_DOWN)  sel.y++;
			}

			sel.x = SDL_clamp(sel.x, 0, dim.x - 1);
			sel.y = SDL_clamp(sel.y, 0, dim.y - 1);
		}

		// Number input (0-9)
		if ((key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_KP_0 && key <= SDLK_KP_9)) {
			int v = 0;
			if (key >= SDLK_0 && key <= SDLK_9)
				v = key - SDLK_0;
			else
				v = key - SDLK_KP_0;

			board[sel.y][sel.x] = v;
		}

		break;
	}
}

static void puzzle_editor_update(float dt) {

}

static void puzzle_editor_draw(void) {
	text_draw_shadow("Puzzle Editor", 10, 10, 3);

	float grid_w = dim.x * CELL;
	float grid_h = dim.y * CELL;
	float gx = (SCREEN_W - grid_w) / 2.0f;
	float gy = (SCREEN_H - grid_h) / 2.0f;

	// background
	draw_set_color(0x202020);
	draw_fill_rect(&RECT(gx-4, gy-4, grid_w+8, grid_h+8));

	for (int y = 0; y < dim.y; y++) {
		for (int x = 0; x < dim.x; x++) {
			SDL_FRect cellrect = RECT(gx + x*CELL, gy + y*CELL, CELL-2, CELL-2);
			draw_set_color((x == sel.x && y == sel.y) ? 0xA6ADDB : 0xFFFFFF);
			draw_fill_rect(&cellrect);

			int v = board[y][x];
			if (v == 0)
				continue;

			font_set_color(color_numbers(v));
			font_draw_char_shadow((char)('0' + v), cellrect.x + 3, cellrect.y - 1, 3);
		}
	}
}

Scene puzzle_editor_scene = {
	"puzzle_editor",
	puzzle_editor_init,
	puzzle_editor_event,
	puzzle_editor_update,
	puzzle_editor_draw,
	CLR_BACKGROUND_1
};

