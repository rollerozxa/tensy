#include "about.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "input.h"
#include "media/raccoon.h"
#include "media/textures.h"

#include <math.h>

static Button ok_button;

void about_init(void) {
	BUTTON(ok_button, RECT(245,300,150,40), "Ok");
}

void about_event(const SDL_Event *ev) {

	if (button_event(ev, &ok_button) || is_escaping(ev))
		switch_scene("mainmenu");
}

static char title[] = "Tensy";

void about_draw(SDL_Renderer *renderer) {
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 12; y++) {
			SDL_RenderTexture(renderer, textures_get(TEX_BG_STRIPES), NULL,
				&RECT(x*32, y*32, 32, 32));
		}
	}

	raccoon_draw(renderer, &POINT(460,50));

	set_font_color(CLR_WHITE);

	for (size_t i = 0; title[i] != '\0'; i++) {
		const int y = 10+sin(SDL_GetTicks()/400.0+i)*12;

		draw_char_shadow(renderer, title[i], 10+i*GLYPH_WIDTH*6, y, 6);
	}

	draw_text_shadow(renderer, "Tensy is a puzzle game where you", 20, 100, 2);
	draw_text_shadow(renderer, "drag to select numbers that sum", 20, 125, 2);
	draw_text_shadow(renderer, "up to ten.", 20, 150, 2);

	draw_text_shadow(renderer, "Well, that's the short version", 20, 200, 2);
	draw_text_shadow(renderer, "of it.", 20, 225, 2);

	draw_text_shadow_centered(renderer, "Game by", &RECT(450,225,180,25), 2);
	draw_text_shadow_centered(renderer, "ROllerozxa", &RECT(450,250,180,25), 2);

	button(renderer, &ok_button);
}

Scene about_scene = {
	"about",
	about_init,
	about_event,
	NULL,
	about_draw,
	0x1F3F8F
};
