#include "debug.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "mouse.h"
#include "text.h"
#include <stdio.h>

static bool debug = false;

void debug_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_F3)
		debug = !debug;
}

void debug_draw(void) {
	if (!debug)
		return;

	font_set_color(CLR_WHITE);

	draw_set_color_alpha(0xFFFFFF30);
	draw_set_blend(true);

	int cellSize = 20;

	for (size_t x = 0; x < NATIVE_WIDTH; x += cellSize)
		draw_line(x, 0, x, NATIVE_HEIGHT);

	for (size_t y = 0; y < NATIVE_HEIGHT; y += cellSize)
		draw_line(0, y, NATIVE_WIDTH, y);

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);
	draw_set_color(0xFFFF00);
	draw_line(mouse.x, 0, mouse.x, NATIVE_HEIGHT);
	draw_line(0, mouse.y, NATIVE_WIDTH, mouse.y);

	FMT_STRING(text, 1024, "%s, using %s driver, cursor pos (%f,%f)",
		SDL_GetPlatform(), SDL_GetRendererName(renderer), mouse.x, mouse.y);
	text_draw_shadow(text, 0, 0, 1);
}
