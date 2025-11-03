#include "tooltip.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "mouse.h"
#include "text.h"
#include <stddef.h>
#include <SDL3/SDL.h>

static bool tooltip_active = false;
static const char *tooltip_text = NULL;

void tooltip_show(const char *text) {
	tooltip_text = text;
	tooltip_active = true;
}

const char *tooltip_get_text(void) {
	return tooltip_text != NULL ? tooltip_text : "";
}

void tooltip_hide(void) {
	tooltip_text = NULL;
	tooltip_active = false;
}

void tooltip_draw(void) {
	if (!tooltip_active)
		return;

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	SDL_FRect tooltip_bg = {0, 0, (strlen(tooltip_text)) * 12 + 20, 32};

	// Clamp position to screen bounds
	tooltip_bg.x = SDL_clamp(mouse.x + 10, 0, SCREEN_W - tooltip_bg.w);
	tooltip_bg.y = SDL_clamp(mouse.y + 10, 0, SCREEN_H - tooltip_bg.h);

	draw_set_blend(true);
	SDL_SetRenderDrawColor(renderer, 13, 13, 13, 235);
	draw_fill_rect(&tooltip_bg);
	draw_set_blend(false);

	font_set_color(CLR_WHITE);
	text_draw_shadow_centered(tooltip_text, &tooltip_bg, 2);
}
