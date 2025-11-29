#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gamesettings.h"
#include "gui/button.h"
#include "input.h"
#include "media/lsdozxa.h"
#include "media/raccoon.h"
#include "media/sound.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <stddef.h>

static Button ok_button, credits_button;

// 0 - raccoon
// 1 - lsdozxa
static int draw_mode = 0;

void about_init(void) {
	BUTTON(ok_button, RECT(245,300,150,40), "Ok");
	BUTTON(credits_button, RECT(470, 280, 140, 40), "Credits");
}

void about_event(const SDL_Event *ev) {
	if (button_event(ev, &credits_button))
		scene_switch("credits");

	if (button_event(ev, &ok_button) || is_escaping(ev))
		scene_switch("mainmenu");

	bool mouse_in_image = SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &RECT(450, 50, 175, 160));
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && mouse_in_image) {
		sound_play(SND_WOOZY);
		draw_mode = (draw_mode + 1) % 2;
	}
}

void about_update(float dt) {
	raccoon_update(dt);
	lsdozxa_update(dt);
}

static char title[] = "Tensy";

void about_draw(void) {
	draw_tiled_bg(0);

	bool should_animate = !settings_getflag(FLAG_REDUCED_MOTION);

	if (draw_mode == 0)
		raccoon_draw(&POINT(460,50), should_animate);
	else
		lsdozxa_draw(&POINT(465,60), should_animate);

	for (size_t i = 0; title[i] != '\0'; i++) {
		const float y = settings_getflag(FLAG_REDUCED_MOTION) ? 10
			: 10 + sin(SDL_GetTicks() / 400.0 + i) * 12 ;

		font_draw_char_shadow(title[i], 10+i*GLYPH_WIDTH*6, y, 6);
	}

	text_draw_shadow("Tensy is a puzzle game where you", 20, 100, 2);
	text_draw_shadow("drag to select numbers that sum", 20, 125, 2);
	text_draw_shadow("up to ten.", 20, 150, 2);

	text_draw_shadow("Well, that's the short version", 20, 200, 2);
	text_draw_shadow("of it.", 20, 225, 2);

	text_draw_shadow_centered("Game by", &RECT(450,225,180,25), 2);
	text_draw_shadow_centered("ROllerozxa", &RECT(450,250,180,25), 2);

	button(&ok_button);
	button(&credits_button);
}

Scene about_scene = {
	"about",
	about_init,
	about_event,
	about_update,
	about_draw,
	0x1F3F8F
};
