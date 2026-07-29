#include "hyperlink.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "media/sound.h"
#include "mouse.h"
#include "text.h"

static float hyperlink_scale = 2; // XXX: should be customisable

bool hyperlink_event(const SDL_Event *ev, Hyperlink *link) {
	SDL_FRect text_rect = text_calc_rect(link->text, hyperlink_scale);
	text_rect.x = link->pos.x;
	text_rect.y = link->pos.y;

	if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &text_rect)) {
		if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			link->_held = true;
		} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && link->_held) {
			link->_held = false;
			sound_play(SND_CLICK);
			return true;
		}
	}

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && link->_held) {
		link->_held = false;
	}

	return false;
}

void hyperlink(Hyperlink *link) {

	SDL_FRect rect = text_calc_rect(link->text, hyperlink_scale);
	rect.x = link->pos.x;
	rect.y = link->pos.y;

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	SDL_Color color = CLR_WHITE;

	if (SDL_PointInRectFloat(&mouse, &rect)) {
		// Draw shadowed underline
		SDL_FRect underline_rect = {
			rect.x + 1, rect.y + rect.h,
			rect.w, 2
		};
		draw_set_color(0x000000);
		draw_fill_rect(&underline_rect);
		underline_rect.y -= 2;
		underline_rect.x -= 2;
		if (link->_held)
			draw_set_color(0xAAAAFF);
		else
			draw_set_color(0xFFFFFF);
		draw_fill_rect(&underline_rect);

		if (link->_held)
			color = color_bitpack_to_sdl(0xAAAAFF);
		else
			color = color_bitpack_to_sdl(0xFFFFFF);
	} else
		color = color_bitpack_to_sdl(0xAAAAFF);

	font_set_color(color);
	text_draw_shadow_centered(link->text, &rect, hyperlink_scale);
}
