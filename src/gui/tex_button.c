#include "tex_button.h"
#include "consts.h"
#include "media/sound.h"
#include "media/textures.h"
#include "mouse.h"

static unsigned char tex_mod(SDL_FPoint *mouse, TexButton *button) {
	if (button->_disabled)
		return 0x70; // Greyed out when disabled

	if (!SDL_PointInRectFloat(mouse, &button->rect))
		return 0xFF; // Not hovered

	if (!button->_held)
		return 0xBB; // Hovering

	return 0x88; // Hovering & button held
}

bool tex_button_event(const SDL_Event *ev, TexButton *button) {
	if (button->_disabled) {
		button->_held = false;
		return false;
	}

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

	unsigned char mod = tex_mod(&mouse, button);
	SDL_SetTextureColorMod(texture, mod, mod, mod);

	SDL_RenderTexture(renderer, texture, NULL, &rect);
}
