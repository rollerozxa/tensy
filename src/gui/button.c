#include "button.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "mouse.h"
#include "render.h"
#include "sound.h"

bool button_event(const SDL_Event *ev, Button *button) {
	if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &button->rect)) {
		if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			button->_held = true;
		} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && button->_held) {
			button->_held = false;
			sound_play(SND_CLICK);
			return true;
		}
	}

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && button->_held) {
		button->_held = false;
	}

	return false;
}

void button(SDL_Renderer *renderer, Button *button) {

	SDL_FRect rect = button->rect;
	const char *text = button->text;

	SDL_FPoint mouse;
	mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);

	if (SDL_PointInRectFloat(&mouse, &rect)) {
		if (button->_held)
			draw_box_active(renderer, &rect);
		else
			draw_box_hover(renderer, &rect);
	} else
		draw_box(renderer, &rect);

	SDL_FRect text_rect = calculate_text_rect(text, 2);

	set_font_color(CLR_WHITE);
	draw_text_shadow(renderer, text, rect.x+(rect.w-(float)text_rect.w)/2, rect.y+(rect.h-(float)text_rect.h)/2, 2);
}
