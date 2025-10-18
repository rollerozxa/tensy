#include "font.h"
#include "gui/button.h"
#include "input.h"
#include "scene.h"
#include <SDL3/SDL.h>
#include "draw.h"
#include "consts.h"
#include "gamesettings.h"
#include "text.h"
#include "color.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static Button back_button;

void statistics_numbers_init(void) {
	BUTTON(back_button, RECT(480, 315, 95, 30), "Back");
}

void statistics_numbers_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button) || is_escaping(ev))
		scene_switch("statistics");
}

void statistics_numbers_update(float dt) {

}

static double bgpan = 0;

void statistics_numbers_draw(void) {
	if (settings_getflag(FLAG_REDUCED_MOTION))
		bgpan = 0;
	else
		bgpan = fmod(bgpan + 0.25, 32);

	draw_tiled_bg(bgpan);

	SDL_FPoint dims = { SCREEN_W * 0.8f, SCREEN_H * 0.75f };
	SDL_FRect panel = draw_centered_fill_rect(dims);

	const int margin = 30;
	SDL_FRect chart = {
		panel.x + margin,
		panel.y + margin,
		panel.w - 2*margin,
		panel.h - 2*margin,
	};

	uint32_t maxv = 1;
	for (int i = 0; i < 9; ++i)
		if (settings()->numbers[i] > maxv)
			maxv = settings()->numbers[i];

	draw_set_color(0x000000);
	draw_fill_rect(&RECT(chart.x - 2.0f, chart.y, 2.0f, chart.h));
	draw_fill_rect(&RECT(chart.x, chart.y + chart.h - 2.0f, chart.w, 2.0f));

	const int spacing = 8;
	const int total_spacing = spacing * 10;
	const int bar_w = (chart.w - total_spacing) / (float)9;
	const int base_y = chart.y + chart.h;

	for (int i = 0; i < 9; ++i) {
		int x = chart.x + spacing + i * (bar_w + spacing);
		int height = ((float)settings()->numbers[i] / (float)maxv) * (chart.h - 24.0f);
		if (height < 1.0f) height = 1.0f;

		SDL_FRect bar = { x, base_y - height, bar_w, height };
		SDL_FRect digit_rect = { x, base_y + 6.0f, bar_w, GLYPH_HEIGHT };
		SDL_FRect count_rect = RECTCPY(digit_rect);
		count_rect.y = bar.y - (GLYPH_HEIGHT*1.5);

		if (settings()->numbers[i] > 0)
			draw_border_box(&bar, color_sdl_to_bitpack(color_numbers(i+1)), 0x000000, 2);

		FMT_STRING(digitbuf, 4, "%d", i+1);
		text_draw_shadow_centered(digitbuf, &digit_rect, 2);

		FMT_STRING(countbuf, 32, "%u", settings()->numbers[i]);
		text_draw_shadow_centered(countbuf, &count_rect, 2);
	}

	button(&back_button);
}

Scene statistics_numbers_scene = {
	"statistics_numbers",
	statistics_numbers_init,
	statistics_numbers_event,
	statistics_numbers_update,
	statistics_numbers_draw,
	0x1F3F8F
};
