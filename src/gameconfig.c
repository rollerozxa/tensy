#include "gameconfig.h"
#include "board.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "scene.h"

typedef struct {
	int w;
	int h;
	const char *name;
	Button button;
} BoardSize;

static BoardSize board_sizes[] = {
	{10,5, "Small"},
	{20,10, "Medium"},
	{30,15, "Big"}
};
static size_t num_board_sizes = sizeof(board_sizes) / sizeof(board_sizes[0]);

static Button go_button;

void gameconfig_init(void) {
	for (size_t i = 0; i < num_board_sizes; i++) {
		int btn_width = 100;
		SDL_FRect button_rect = {150 + i * (btn_width + 15), 80, btn_width, 40};

		BUTTON(board_sizes[i].button, button_rect, board_sizes[i].name);
	}

	BUTTON(go_button, RECT(0,NATIVE_HEIGHT-40, NATIVE_WIDTH, 40), "Play");

	board_change_size(board.w, board.h, 1);
	board_zerofill();
}

void gameconfig_event(const SDL_Event *ev) {


	for (size_t i = 0; i < num_board_sizes; i++) {
		BoardSize *board_size = &board_sizes[i];
		if (button_event(ev, &board_size->button)) {
			board_change_size(board_size->w, board_size->h, board.scale);
			board_zerofill();
		}

	}

	if (button_event(ev, &go_button)) {
		switch_scene("game");
	}

}

void gameconfig_update(void) {

}

void gameconfig_draw(SDL_Renderer *renderer) {
	set_font_color(CLR_WHITE);
	draw_text_shadow(renderer, "Configure game", 20, 20, 3);

	board_draw(renderer, false);

	SDL_FRect label_rect = RECTCPY(board_sizes[0].button.rect);
	label_rect.x = 20;
	draw_text_shadow_centered(renderer, "Board size", &label_rect, 2);
	for (size_t i = 0; i < num_board_sizes; i++) {
		button(renderer, &board_sizes[i].button);
	}

	button(renderer, &go_button);
}

Scene gameconfig_scene = {
	"gameconfig",
	gameconfig_init,
	gameconfig_event,
	gameconfig_update,
	gameconfig_draw,
	0x1F3F8F
};
