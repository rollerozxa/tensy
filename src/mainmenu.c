#include "mainmenu.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "scene.h"
#include "textures.h"

#include <math.h>
#include <stdio.h>

#define BOARD_W 30
#define BOARD_H 16

static int temp_board[BOARD_W][BOARD_H];

static int step = 0;

static Button play_button = {RECT(245,200,150,40), "Play"};
static Button settings_button = {RECT(245,260,150,40), "Settings"};

void mainmenu_init(void) {
	step = 0;
}

void mainmenu_event(const SDL_Event *ev) {
	if (button_event(ev, &play_button)) {
		switch_scene("game");
	}

	if (button_event(ev, &settings_button)) {
		switch_scene("settings");
	}

	if (ev->type == SDL_EVENT_KEY_UP && ev->key.key == SDLK_AC_BACK) {
		switch_scene("exiting");
	}
}

void mainmenu_update(void) {
	if (step % 5 == 0) {
		SDL_Point rand_pos;
		while (true) {
			rand_pos = (struct SDL_Point){SDL_rand(BOARD_W), SDL_rand(BOARD_H)};

			if (temp_board[rand_pos.x][rand_pos.y] != 0)
				continue;

			break;
		}

		temp_board[rand_pos.x][rand_pos.y] = SDL_rand(9)+1;

		temp_board[SDL_rand(BOARD_W)][SDL_rand(BOARD_H)] = 0;
	}

	step++;
}

static char title[] = "Tensy";
static double bgpan = 0;

void mainmenu_draw(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0x1f, 0x3f, 0x8f, 0xFF);

	SDL_RenderClear(renderer);

	bgpan = fmod(bgpan + 0.25, 32);

	for (int x = -1; x < 20; x++) {
		for (int y = -1; y < 12; y++) {
			SDL_RenderTexture(renderer, textures_get(TEX_BG_STRIPES), NULL,
				RECT(x*32+bgpan, y*32+bgpan, 32, 32));
		}
	}

#if 0
	for (size_t x = 0; x < BOARD_W; x++) {
		for (size_t y = 0; y < BOARD_H; y++) {
			if (temp_board[x][y] == 0)
				continue;

			set_font_color(num_to_colour(temp_board[x][y]));
			draw_char_shadow(renderer, temp_board[x][y]+'0', 3+(x+1)*20, (y+1)*20-1, 2);
		}
	}
#endif

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	for (size_t i = 0; title[i] != '\0'; i++) {
		const int y = 20+sin(SDL_GetTicks()/400.0+i)*16;

		draw_char_shadow(renderer, title[i], 190+i*GLYPH_WIDTH*8, y, 8);
	}

	button(renderer, &play_button);
	button(renderer, &settings_button);
}
