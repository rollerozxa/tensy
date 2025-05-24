#include "tex_button.h"
#include "consts.h"
#include "media/sound.h"
#include "media/textures.h"
#include "mouse.h"

bool tex_button_event(const SDL_Event *ev, TexButton *button) {
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

void tex_button(SDL_Renderer *renderer, TexButton *button) {

	SDL_FRect rect = button->rect;
	SDL_Texture *texture = textures_get(button->texture);

	SDL_FPoint mouse;
	mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);

	if (SDL_PointInRectFloat(&mouse, &rect)) {
		if (button->_held)
			SDL_SetTextureColorMod(texture, 0x88, 0x88, 0x88);
		else
			SDL_SetTextureColorMod(texture, 0xBB, 0xBB, 0xBB);
	} else
		SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);

	SDL_RenderTexture(renderer, texture, NULL, &rect);
}
