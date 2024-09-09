#include "game.h"
#include "consts.h"
#include "font.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int number;
	bool removed;
} Cell;

typedef struct {
	int x;
	int y;
} Neighbour;

#define BOARD_W 32
#define BOARD_H 18

Cell board[BOARD_W][BOARD_H];

bool helddown = false;
SDL_Point first_held_pos = {-1,-1};
SDL_Point current_held_pos = {-1,-1};

void calculate_move() {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
		for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
			if (!board[x][y].removed)
				sum += board[x][y].number;
		}
	}

	printf("%d\n", sum);

	if (sum == 10) {
		for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
			for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
				board[x][y].removed = true;
			}
		}
	}
}

void game_init(void) {
	for (size_t x = 0; x < BOARD_W; x++) {
		for (size_t y = 0; y < BOARD_H; y++) {
			board[x][y].number = rand() % 9 + 1;
			board[x][y].removed = false;
		}
	}
}

void game_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float mx = ev->button.x;
		float my = ev->button.y;

		first_held_pos.x = floorf(mx / 20);
		first_held_pos.y = floorf(my / 20);

		current_held_pos.x = first_held_pos.x;
		current_held_pos.y = first_held_pos.y;

		helddown = true;

		printf("%f, %f\n", mx, my);
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
		if (helddown) {
			float mx = ev->motion.x;
			float my = ev->motion.y;

			current_held_pos.x = floorf(mx / 20);
			current_held_pos.y = floorf(my / 20);
		}
	} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		calculate_move();

		first_held_pos.x = -1;
		first_held_pos.y = -1;

		current_held_pos.x = -1;
		current_held_pos.y = -1;

		helddown = false;
	}
}

void game_update(void) {


}

void game_draw(SDL_Renderer *renderer) {
	//SDL_SetRenderDrawColor(renderer, 0xAA, 0x22, 0x22, 0xFF);
	//SDL_RenderFillRect(renderer, RECT(first_held_pos.x*20,first_held_pos.y*20,20,20));

	//SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0xAA, 0xFF);
	//SDL_RenderFillRect(renderer, RECT(current_held_pos.x*20,current_held_pos.y*20,20,20));

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderRect(renderer,
		RECT(fminf(first_held_pos.x*20, current_held_pos.x*20),
		fminf(first_held_pos.y*20, current_held_pos.y*20),
		abs((current_held_pos.x - first_held_pos.x) * 20)+20,
		abs((current_held_pos.y - first_held_pos.y) * 20)+20));

	for (size_t x = 0; x < BOARD_W; x++) {
		for (size_t y = 0; y < BOARD_H; y++) {
			if (!board[x][y].removed)
				draw_char(renderer, board[x][y].number+'0', 3+x*20, y*20-1, 2);
		}
	}
}
