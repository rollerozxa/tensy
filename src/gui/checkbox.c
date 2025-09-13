#include "checkbox.h"

#include <stddef.h>

#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "media/sound.h"
#include "media/textures.h"
#include "mouse.h"
#include "text.h"

#define CALCULATE_RECTS() \
	SDL_FRect label_rect = text_calc_rect(label, 2); \
	SDL_FRect *check_rect = &RECT(pos.x+2, pos.y+2, 20, 20); \
	SDL_FRect full_rect = {pos.x, pos.y, 24+26+label_rect.w, label_rect.h}

bool checkbox_event(const SDL_Event *ev, Checkbox *checkbox) {
	SDL_FPoint pos = checkbox->pos;
	const char *label = checkbox->label;

	CALCULATE_RECTS();

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP
			&& SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &full_rect)) {
		checkbox->checked = !checkbox->checked;
		sound_play(SND_CLICK);
		return true;
	}

	return false;
}

bool checkbox(Checkbox *checkbox) {

	SDL_FPoint pos = checkbox->pos;
	const char *label = checkbox->label;

	SDL_FPoint mouse;
	int clicked = mouse_get_state_scaled(&mouse);

	CALCULATE_RECTS();

	if (SDL_PointInRectFloat(&mouse, &full_rect)) {
		if (clicked & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
			draw_box_active(check_rect);
		else
			draw_box_hover(check_rect);
	} else
		draw_box(check_rect);

	if (checkbox->checked) {
		draw_texture(TEX_CHECK, NULL,
			&RECT(check_rect->x, check_rect->y, 20, 20));
	}

	font_set_color(CLR_WHITE);
	text_draw_shadow(label, pos.x+26, pos.y, 2);

	return checkbox->checked;
}
