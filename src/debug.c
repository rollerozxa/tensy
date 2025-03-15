#include "debug.h"
#include "consts.h"
#include "font.h"
#include "mouse.h"
#include "render.h"
#include <stdio.h>

static bool debug = false;

void debug_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_F3)
		debug = !debug;
}

void debug_draw(SDL_Renderer *renderer) {
	if (!debug)
		return;

	set_draw_color_alpha(renderer, 0xFFFFFF30);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int cellSize = 20;

	for (size_t x = 0; x < NATIVE_WIDTH; x += cellSize)
		SDL_RenderLine(renderer, x, 0, x, NATIVE_HEIGHT);

	for (size_t y = 0; y < NATIVE_HEIGHT; y += cellSize)
		SDL_RenderLine(renderer, 0, y, NATIVE_WIDTH, y);

	SDL_FPoint mouse;
	mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);
	set_draw_color(renderer, 0xFFFF00);
	SDL_RenderLine(renderer, mouse.x, 0, mouse.x, NATIVE_HEIGHT);
	SDL_RenderLine(renderer, 0, mouse.y, NATIVE_WIDTH, mouse.y);

	char text[1024];
	snprintf(text, 1023, "%s, using %s driver, cursor pos (%f,%f)",
		SDL_GetPlatform(), SDL_GetRendererName(renderer), mouse.x, mouse.y);
	draw_text_shadow(renderer, text, 0, 0, 1);
}
