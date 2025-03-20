#include "mainmenu.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "render.h"
#include "scene.h"
#include "textures.h"

#include <math.h>
#include <stdio.h>

#define BOARD_W 30
#define BOARD_H 16

static Button play_button, settings_button;

void mainmenu_init(void) {
	BUTTON(play_button, RECT(245,200,150,40), "Play");
	BUTTON(settings_button, RECT(245,260,150,40), "Settings");
}

void mainmenu_event(const SDL_Event *ev) {
	if (button_event(ev, &play_button))
		switch_scene("gameconfig");

	if (button_event(ev, &settings_button))
		switch_scene("settings");

	if (ev->type == SDL_EVENT_KEY_UP && ev->key.key == SDLK_AC_BACK)
		switch_scene("exiting");
}

void mainmenu_update(void) {

}

static char title[] = "Tensy";
static double bgpan = 0;

void mainmenu_draw(SDL_Renderer *renderer) {
	bgpan = fmod(bgpan + 0.25, 32);

	for (int x = -1; x < 20; x++) {
		for (int y = -1; y < 12; y++) {
			SDL_RenderTexture(renderer, textures_get(TEX_BG_STRIPES), NULL,
				&RECT(x*32+bgpan, y*32+bgpan, 32, 32));
		}
	}

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	for (size_t i = 0; title[i] != '\0'; i++) {
		const float y = 20+sin(SDL_GetTicks()/400.0+i)*16;

		draw_char_shadow(renderer, title[i], 190+i*GLYPH_WIDTH*8, y, 8);
	}

	button(renderer, &play_button);
	button(renderer, &settings_button);
}

Scene mainmenu_scene = {
	"mainmenu",
	mainmenu_init,
	mainmenu_event,
	mainmenu_update,
	mainmenu_draw,
	0x1F3F8F
};
