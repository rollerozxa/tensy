#include "button.h"
#include "consts.h"
#include "font.h"
#include "mouse.h"

#include <stdio.h>

bool button_event(const SDL_Event *ev, Button *button) {
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP
			&& SDL_PointInRectFloat(&(SDL_FPoint){ev->motion.x, ev->motion.y}, button->rect)) {
		return true;
	}

	return false;
}

void button(SDL_Renderer *renderer, Button *button) {

	SDL_FRect *rect = button->rect;
	const char *text = button->text;

	SDL_FPoint mouse;
	int clicked = mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);

	SDL_SetRenderDrawColor(renderer, 0x13, 0x13, 0x13, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x, rect->y, rect->w, rect->h));

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+1, rect->y+1, rect->w-2, rect->h-2));

	SDL_SetRenderDrawColor(renderer, 0xc6, 0xc6, 0xc6, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+1, rect->y+rect->h-2, 1, 1));
	SDL_RenderFillRect(renderer, RECT(rect->x+rect->w-2, rect->y+1, 1, 1));

	SDL_SetRenderDrawColor(renderer, 0x58, 0x56, 0x58, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-3, rect->h-3));

	if (SDL_PointInRectFloat(&mouse, rect)) {
		if (clicked & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
			SDL_SetRenderDrawColor(renderer, 0x74, 0x74, 0x74, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0xa6, 0xad, 0xdb, 0xFF);
	} else
		SDL_SetRenderDrawColor(renderer, 0xa2, 0xa2, 0xa2, 0xFF);

	printf("%fx%f, %fx%f\n", rect->x, rect->y, rect->w, rect->h);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));

	SDL_Rect text_rect = calculate_text_rect(text, 2);

	set_font_color((SDL_Color){0x00, 0x00, 0x00});
	draw_text(renderer, text, rect->x+(rect->w-(float)text_rect.w)/2, rect->y+(rect->h-(float)text_rect.h)/2, 2);
}
