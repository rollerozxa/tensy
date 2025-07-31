#include "gameconfig.h"
#include "board.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "gamestate.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "input.h"
#include "scene.h"

typedef struct {
	int w;
	int h;
	float scale;
	const char *name;
	Button button;
} BoardSize;

static BoardSize board_sizes[] = {
	{15,8,  3,   "Small"},
	{20,10, 2.5, "Medium"},
	{30,15, 2,   "Big"},
	{60,30, 1, "Hyuge!"}
};
static size_t num_board_sizes = sizeof(board_sizes) / sizeof(board_sizes[0]);

static bool show_hyuge = false;

static Board board_preview = {NULL, 30, 15, 2};

static Button go_button;
static Checkbox physics_checkbox;

void gameconfig_init(void) {
	for (size_t i = 0; i < num_board_sizes; i++) {
		int btn_width = 100;
		SDL_FRect button_rect = {150 + i * (btn_width + 15), 75, btn_width, 40};

		BUTTON(board_sizes[i].button, button_rect, board_sizes[i].name);
	}

	BUTTON(go_button, RECT(0,NATIVE_HEIGHT-40, NATIVE_WIDTH, 40), "Play");

	CHECKBOX(physics_checkbox, POINT(20, 140), false, "Board physics");

	board_preview.rect_offset = POINT(150, 50);
	board_change_size(&board_preview, board_sizes[2].w, board_sizes[2].h, board_sizes[2].scale -1);
	board_zerofill(&board_preview);
}

void gameconfig_event(const SDL_Event *ev) {

	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_LSHIFT)
		show_hyuge = true;
	else if (ev->type == SDL_EVENT_KEY_UP && ev->key.scancode == SDL_SCANCODE_LSHIFT)
		show_hyuge = false;

	if (is_escaping(ev))
		switch_scene("selectmode");

	for (size_t i = 0; i < num_board_sizes; i++) {
		BoardSize *board_size = &board_sizes[i];
		if (button_event(ev, &board_size->button)) {
			board_change_size(&game.board, board_size->w, board_size->h, board_size->scale);
			board_change_size(&board_preview, board_size->w, board_size->h, SDL_max(0.5f, board_size->scale - 1));
			board_zerofill(&game.board);
			board_zerofill(&board_preview);
		}
	}

	if (button_event(ev, &go_button))
		switch_scene("game");

	if (current_gamemode().gravity_mode == GRA_Ask && checkbox_event(ev, &physics_checkbox))
		game.board.physics = physics_checkbox.checked;
}

void gameconfig_draw(SDL_Renderer *renderer) {
	set_font_color(CLR_WHITE);
	draw_text_shadow(renderer, "Configure game", 10, 10, 3);

	board_draw(&board_preview, renderer, false);

	SDL_FRect label_rect = RECTCPY(board_sizes[0].button.rect);
	label_rect.x = 25;
	draw_text_shadow_centered(renderer, "Board size", &label_rect, 2);
	for (size_t i = 0; i < num_board_sizes; i++) {
		if (i == 3 && !show_hyuge)
			continue;

		button(renderer, &board_sizes[i].button);
	}

	if (current_gamemode().gravity_mode == GRA_Ask)
		checkbox(renderer, &physics_checkbox);

	button(renderer, &go_button);
}

Scene gameconfig_scene = {
	"gameconfig",
	gameconfig_init,
	gameconfig_event,
	NULL,
	gameconfig_draw,
	0x1F3F8F
};
