#include "slider.h"
#include "consts.h"
#include "font.h"
#include "mouse.h"
#include "render.h"
#include <stdio.h>

void calculate_handle_pos(const SDL_Event *ev, Slider *slider) {
	slider->_pos = ev->motion.x - slider->rect->x - ((float)slider->_handle_width/2);
	slider->_pos = SDL_clamp(slider->_pos, 0, slider->rect->w - slider->_handle_width);

	slider->value = slider->min + SDL_round(slider->_pos / ((slider->rect->w - slider->_handle_width) / (slider->max - slider->min)));

	// Trying to make it snap to each integer value...
	//slider->_pos = SDL_round(slider->_pos / ((slider->rect->w - slider->_handle_width) / (slider->max-slider->min)));
	//slider->_pos = slider->_pos * (slider->rect->w / (slider->max-slider->min+1));
}

bool slider_event(const SDL_Event *ev, Slider *slider) {

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (SDL_PointInRectFloat(&(SDL_FPoint){ev->motion.x, ev->motion.y}, slider->rect)) {
			slider->_dragging = true;
			calculate_handle_pos(ev, slider);
		}
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
		if (slider->_dragging) {
			calculate_handle_pos(ev, slider);
		}
	} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		slider->_dragging = false;
	}

	return false;
}

void slider(SDL_Renderer *renderer, Slider *slider) {

	SDL_FRect *rect = slider->rect;

	slider->_handle_width = rect->w / (slider->max - slider->min);

	SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x, rect->y, rect->w, rect->h));

	SDL_FPoint mouse;
	int clicked = mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);

	SDL_FRect handle_rect = {rect->x+slider->_pos, rect->y, slider->_handle_width, rect->h};

	if (SDL_PointInRectFloat(&mouse, rect)) {
		if (clicked & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
			draw_box_active(renderer, &handle_rect);
		else
			draw_box_hover(renderer, &handle_rect);
	} else
		draw_box(renderer, &handle_rect);

	char dbg[512];
	snprintf(dbg, 511, "%d", slider->value);
	draw_text(renderer, dbg, rect->x + rect->w + 10, rect->y, 2);
}
