#include "button.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "media/sound.h"
#include "mouse.h"
#include "text.h"

static float label_scale(float btn_height) {
	return btn_height <= 25 ? 1.5 : 2;
}

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

void button(Button *button) {

	SDL_FRect rect = button->rect;
	const char *text = button->text;

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	if (button->_disabled)
		draw_box_disabled(&rect);
	else if (SDL_PointInRectFloat(&mouse, &rect)) {
		if (button->_held)
			draw_box_active(&rect);
		else
			draw_box_hover(&rect);
	} else
		draw_box(&rect);

	SDL_FRect text_rect = text_calc_rect(text, label_scale(rect.h));

	font_set_color(CLR_WHITE);
	text_draw_shadow_centered(text, &rect, label_scale(rect.h));
}

void button_bar(Button *buttons, float x, float y, float yp) {
	for (size_t i = 0; buttons[i].rect.w != 0; i++) {
		Button *btn = &buttons[i];

		btn->rect.x = x;
		btn->rect.y = y + i * (btn->rect.h + yp);
		button(&buttons[i]);
	}
}
