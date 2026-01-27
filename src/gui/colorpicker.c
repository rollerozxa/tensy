#include "colorpicker.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "mouse.h"
#include <SDL3/SDL.h>
#include <math.h>

static void update_color(ColorPicker *picker) {
	// update HSV selection from new colour
	SDL_Color sc = color_bitpack_to_sdl(picker->color);
	double r = sc.r / 255.0;
	double g = sc.g / 255.0;
	double b = sc.b / 255.0;
	double max = SDL_max(SDL_max(r, g), b);
	double min = SDL_min(SDL_min(r, g), b);
	double delta = max - min;

	picker->val = max;
	if (max <= 0.0) {
		picker->sat = 0.0;
		picker->hue = 0.0;
	} else {
		picker->sat = (max == 0.0) ? 0.0 : (delta / max);
		if (delta == 0.0)
			picker->hue = 0.0;
		else if (max == r)
			picker->hue = fmod(((g - b) / delta), 6.0) / 6.0;
		else if (max == g)
			picker->hue = (((b - r) / delta) + 2.0) / 6.0;
		else
			picker->hue = (((r - g) / delta) + 4.0) / 6.0;

		if (picker->hue < 0.0)
			picker->hue += 1.0;
	}
}

void colorpicker_init(ColorPicker *picker, const char *initial_hex) {
	SDL_FRect grid = picker->rect;

	// initialize HSV and selection from the color
	picker->color = initial_hex ? parse_hex_color_bitpack(initial_hex) : 0xFFFFFF;
	update_color(picker);

	char buf[8] = {0};
	color_to_hex6(picker->color, buf);

	// Init text input
	SDL_FRect input_rect = {grid.x + grid.w + 8, grid.y + 48 + 8, 96, 28};
	TEXTINPUT(picker->hex, input_rect, buf);
	picker->hex.max_length = 6;

	picker->_slider_rect = (SDL_FRect){grid.x, grid.y + grid.h + 8, grid.w, 18.f};
	picker->_dragging = false;
	picker->_dragging_hue = false;
}

bool colorpicker_event(const SDL_Event *ev, ColorPicker *picker) {

	SDL_FRect grid = picker->rect;
	SDL_FRect slider = picker->_slider_rect;

	// Mouse down handling
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		SDL_FPoint pt = POINT(ev->motion.x, ev->motion.y);
		if (SDL_PointInRectFloat(&pt, &grid))
			picker->_dragging = true;
		else if (SDL_PointInRectFloat(&pt, &slider))
			picker->_dragging_hue = true;
	}

	if (picker->_dragging || picker->_dragging_hue) {
		if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
			picker->_dragging = false;
			picker->_dragging_hue = false;
			return false;
		}

		if (ev->type != SDL_EVENT_MOUSE_MOTION && ev->type != SDL_EVENT_MOUSE_BUTTON_DOWN)
			return false;

		SDL_FPoint mouse;
		mouse_get_state_scaled(&mouse);
		char buf[8];

		if (picker->_dragging) {
			float x = SDL_clamp(mouse.x - grid.x, 0, grid.w);
			float y = SDL_clamp(mouse.y - grid.y, 0, grid.h);

			// sat is x proportion, val is inverted y proportion
			picker->sat = x / grid.w;
			picker->val = 1.0 - (y / grid.h);
		} else if (picker->_dragging_hue) {
			float x = SDL_clamp(mouse.x - slider.x, 0, slider.w);

			picker->hue = x / slider.w;
		}

		SDL_Color c = HSVtoRGB(picker->hue, picker->sat, picker->val);
		picker->color = color_sdl_to_bitpack(c);
		color_to_hex6(picker->color, buf);
		textinput_settext(&picker->hex, buf);
		return true;
	}

	// Let the hex input handle events first
	if (textinput_event(ev, &picker->hex)) {
		// If input lost focus (user pressed enter), parse the text
		if (!picker->hex._focused) {
			unsigned int col = parse_hex_color_bitpack(picker->hex.buffer);
			picker->color = col;
			update_color(picker);
			return true;
		}

		return false;
	}

	return false;
}

void colorpicker(ColorPicker *picker) {

	SDL_FRect grid = picker->rect;
	SDL_FRect slider = picker->_slider_rect;

	// Draw color grid
	for (int j = 0; j < grid.h; j++) {
	for (int i = 0; i < grid.w; i++) {
		SDL_Color c = HSVtoRGB(
			picker->hue,
			(i + 0.5) / (double)grid.w, // saturation across x
			1.0 - ((j + 0.5) / (double)grid.h)); // value downwards

		draw_set_color(color_sdl_to_bitpack(c));
		draw_fill_rect(&RECT(
			grid.x + i,
			grid.y + j,
			1, 1));
	}}

	// Draw selection box for color grid
	draw_set_color(0xFFFFFF);
	draw_rect(&RECT(
		(grid.x + picker->sat * grid.w) - 3,
		(grid.y + (1.0 - picker->val) * grid.h) - 3,
		6, 6
	));

	// Draw horizontal hue slider below the grid
	for (int i = 0; i < slider.w; i++) {
		SDL_Color c = HSVtoRGB(
			(i + 0.5) / (double)slider.w,
			1.0,
			1.0);

		draw_set_color(color_sdl_to_bitpack(c));
		draw_fill_rect(&RECT(slider.x + i, slider.y, 1, slider.h));
	}

	// Draw selection box for slider
	draw_set_color(0xFFFFFF);
	draw_rect(&RECT(
		(slider.x + picker->hue * slider.w) - 4,
		slider.y - 2,
		8, slider.h + 4));

	// Draw preview box to right of grid
	SDL_FRect preview = {grid.x + grid.w + 8, grid.y, 48, 48};
	draw_box(&preview);
	draw_set_color(picker->color);
	draw_fill_rect(&RECT(preview.x + 2, preview.y + 2, preview.w - 4, preview.h - 4));

	textinput(&picker->hex);
}
