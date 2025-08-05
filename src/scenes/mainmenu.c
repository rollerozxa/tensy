#include "mainmenu.h"
#include "app.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gui/button.h"
#include "input.h"
#include "media/textures.h"
#include "scene.h"
#include "text.h"

#include <math.h>
#include <stdio.h>

#define BOARD_W 30
#define BOARD_H 16

static Button play_button, about_button, settings_button;

void mainmenu_init(void) {
	BUTTON(play_button, RECT(245,200,150,40), "Play");
	BUTTON(about_button, RECT(160,260,150,40), "About");
	BUTTON(settings_button, RECT(330,260,150,40), "Settings");
}

void mainmenu_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_UP) {
		if (ev->key.key == SDLK_RETURN || ev->key.key == SDLK_RETURN2)
			scene_switch("selectmode");
	}

	if (button_event(ev, &play_button))
		scene_switch("selectmode");

	if (button_event(ev, &about_button))
		scene_switch("about");

	if (button_event(ev, &settings_button))
		scene_switch("settings");

	if (is_escaping(ev))
		AppQuit();
}

static char title[] = "Tensy";
static double bgpan = 0;

void mainmenu_draw(void) {
	bgpan = fmod(bgpan + 0.25, 32);

	for (int x = -1; x < 20; x++) {
		for (int y = -1; y < 12; y++) {
			draw_texture(TEX_BG_STRIPES, NULL, &RECT(x*32+bgpan, y*32+bgpan, 32, 32));
		}
	}

	for (size_t i = 0; title[i] != '\0'; i++) {
		const float y = 20+sin(SDL_GetTicks()/400.0+i)*16;

		font_draw_char_shadow(title[i], 190+i*GLYPH_WIDTH*8, y, 8);
	}

	button(&play_button);
	button(&about_button);
	button(&settings_button);

	char statusmsg[512];
	snprintf(statusmsg, 511, "Tensy ver. 1.0-dev (%s)", SDL_GetPlatform());
	text_draw_shadow(statusmsg, 0, NATIVE_HEIGHT-12, 1);
}

Scene mainmenu_scene = {
	"mainmenu",
	mainmenu_init,
	mainmenu_event,
	NULL,
	mainmenu_draw,
	0x1F3F8F
};
