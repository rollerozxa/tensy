#include "dropdown.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "media/sound.h"
#include "mouse.h"
#include "text.h"

static float label_scale(float height) {
	return height <= 25 ? 1.5f : 2.0f;
}

#define DROPDOWN_BTN_WIDTH 32

bool dropdown_event(const SDL_Event *ev, Dropdown *dd) {
	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	SDL_FRect rect = dd->rect;

	if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &rect)) {
		if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			dd->_held = true;
		} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && dd->_held) {
			dd->_held = false;
			dd->_open = !dd->_open;
			sound_play(SND_CLICK);
			return true;
		}
	}

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && dd->_held)
		dd->_held = false;

	if (!dd->_open)
		return false;

	for (int i = 0; i < dd->item_count; i++) {
		SDL_FRect itemrect = RECTCPY(rect);
		itemrect.y += rect.h * (i + 1);

		if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &itemrect)) {
			if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				dd->_hover_index = i;
			} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
				dd->selected = i;
				dd->_open = false;
				sound_play(SND_CLICK);
				return true;
			}
		}
	}

	SDL_FRect totalrect = RECTCPY(rect);
	totalrect.h *= dd->item_count + 1;

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && !SDL_PointInRectFloat(&mouse, &totalrect))
		dd->_open = false;

	return false;
}

void dropdown(Dropdown *dropdown) {
	SDL_FRect rect = RECTCPY(dropdown->rect);

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	rect.w -= DROPDOWN_BTN_WIDTH;
	draw_box_disabled(&rect);
	rect.w += DROPDOWN_BTN_WIDTH;

	SDL_FRect dropdownbtn = {
		rect.x + rect.w - DROPDOWN_BTN_WIDTH,
		rect.y,
		DROPDOWN_BTN_WIDTH,
		rect.h
	};

	if (SDL_PointInRectFloat(&mouse, &rect)) {
		if (dropdown->_held)
			draw_box_active(&dropdownbtn);
		else
			draw_box_hover(&dropdownbtn);
	} else
		draw_box(&dropdownbtn);

	// ???
	dropdownbtn.x -= 2;

	text_draw_shadow_centered(dropdown->_open ? "^" : "V", &dropdownbtn, label_scale(dropdownbtn.h));

	float text_height = CENTER(rect.h, text_calc_rect("e", label_scale(rect.h)).h);

	font_set_color(CLR_WHITE);
	if (dropdown->selected >= 0 && dropdown->selected < dropdown->item_count)
		text_draw_shadow(dropdown->items[dropdown->selected], rect.x + 5, rect.y + text_height, label_scale(rect.h));

	if (dropdown->_open) {
		SDL_FRect totalrect = RECTCPY(rect);
		totalrect.y += totalrect.h;
		totalrect.h *= dropdown->item_count;
		draw_border_box(&totalrect, 0x212121, 0x131313, 1);

		for (int i = 0; i < dropdown->item_count; i++) {
			SDL_FRect itemrect = RECTCPY(rect);
			itemrect.y += rect.h * (i + 1);

			if (SDL_PointInRectFloat(&mouse, &itemrect)) {
				draw_set_blend(true);
				draw_set_color_alpha(0xccccff30);
				draw_fill_rect(&itemrect);
				draw_set_color(0xffffff);
			}

			font_set_color(CLR_WHITE);
			text_draw_shadow(dropdown->items[i], itemrect.x + 5, itemrect.y + text_height, label_scale(itemrect.h));
		}
	}
}
