#include "color.h"
#include "consts.h"
#include "font.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stddef.h>

void devintro_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_UP || ev->type == SDL_EVENT_MOUSE_BUTTON_UP)
		scene_switch("intro");
}

static void print_center(const char *text, float y, float scale) {
	SDL_FRect rect = text_calc_rect(text, scale);
	text_draw_shadow(text, CENTER(SCREEN_W, rect.w), y, scale);
}

void devintro_draw(void) {
	font_set_color(color_bitpack_to_sdl(0xFF0000));
	print_center("NOTE:", 25, 3);

	font_set_color(CLR_WHITE);

	print_center("This is a development build of Tensy, showcasing", 80, 2);
	print_center("the game in its current state of development.", 80+30*1, 2);

	print_center("Things are work in progress and may change at", 80+30*3, 2);
	print_center("any point in time during development.", 80+30*4, 2);

	print_center("Press anything or anywhere to continue...", SCREEN_H-50, 2);
}

Scene devintro_scene = {
	"devintro",
	NULL,
	devintro_event,
	NULL,
	devintro_draw,
	0x151515
};
