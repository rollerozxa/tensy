#include "checkbox.h"
#include "colours.h"
#include "font.h"
#include "mouse.h"
#include "render.h"

#define CALCULATE_RECTS() \
	SDL_Rect label_rect = calculate_text_rect(label, 2); \
	SDL_FRect *check_rect = &((SDL_FRect){pos->x+2, pos->y+2, 20, 20}); \
	SDL_FRect full_rect = {pos->x, pos->y, 24+26+label_rect.w, label_rect.h}

bool checkbox_event(const SDL_Event *ev, Checkbox *checkbox) {
	SDL_FPoint *pos = checkbox->pos;
	const char *label = checkbox->label;

	CALCULATE_RECTS();

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP
			&& SDL_PointInRectFloat(&(SDL_FPoint){ev->motion.x, ev->motion.y}, &full_rect)) {
		checkbox->checked = !checkbox->checked;
		return true;
	}

	return false;
}

bool checkbox(SDL_Renderer *renderer, Checkbox *checkbox) {

	SDL_FPoint *pos = checkbox->pos;
	const char *label = checkbox->label;

	SDL_FPoint mouse;
	int clicked = mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);

	CALCULATE_RECTS();

	if (SDL_PointInRectFloat(&mouse, &full_rect)) {
		if (clicked & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
			draw_box_active(renderer, check_rect);
		else
			draw_box_hover(renderer, check_rect);
	} else
		draw_box(renderer, check_rect);

	if (checkbox->checked) {
		set_font_color(CLR_BLACK);
		draw_text(renderer, "V", check_rect->x+3, check_rect->y-1, 2);
	}

	set_font_color(CLR_WHITE);
	draw_text_shadow(renderer, label, pos->x+26, pos->y, 2);

	return checkbox->checked;
}