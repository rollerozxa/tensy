#include "credits.h"

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>

#include "consts.h"
#include "draw.h"
#include "input.h"
#include "media/music.h"
#include "media/textures.h"
#include "text.h"

static const char *credits_text[] = {
"Tensy",
"",
"------------",
"",
"",
"Programming",
"---",
"ROllerozxa",
"",
"",
"Graphics",
"---",
"ROllerozxa",
"Datagoblin (monogram font)",
"",
"",
"Sound",
"---",
"ROllerozxa",
"jsfxr (by chr15m)",
"Various freesound.org authors (CC0)",
"",
"",
"Music",
"---",
"Main Menu Theme",
"cs127 - Digital Rectal Exam",
"",
"Credits Theme",
"Andreas Viklund - Allt jag vill",
"",
"",
"Third-party libraries used",
"---",
"SDL & SDL_mixer (zlib)",
"Sam Lantinga, Ryan Gordon, et al.",
"",
"QOI (MIT)",
"Dominic Szablewski",
"",
"libxmp (MIT)",
"Claudio Matsuoka, Hipolito Carraro Jr, et al.",
"",
"",
"Special thanks to",
"---",
"krisbug",
"",
"",
"",
"",
"---",
"For a full catalogue of licensing for",
"libraries and assets used in the game,",
"see",
"tensy.voxelmanip.se/attribution (TODO)",
"---",
"",
"",
"",
"",
"",
"\"Have some cheesy quote here, or something\"",
"-Some clever person",
"",
"",
"",
"",
"",
"Copyleft MMXXV ROllerozxa",
"",
"All events in the game are fictional,",
"unless stated otherwise.",
"",
"",
};
#define NUM_CREDITS_LINES (sizeof(credits_text)/sizeof(credits_text[0]))

static float scroll_y = NATIVE_HEIGHT;
static float scroll_speed = 25.0f;
static float wave_amplitude = 12.0f;
static float wave_frequency = 4.0f;

static int phase = 1;
static float introcard_timeout = 0.0f;
static float introcard_opacity = 0;

void credits_init(void) {
	music_play(MUS_CREDITS, -1);
	scroll_y = NATIVE_HEIGHT;
	phase = 1;
	introcard_timeout = 0;
	introcard_opacity = 0;
}

void credits_event(const SDL_Event *ev) {
	if (is_escaping(ev)) {
		music_fade_out(1000);
		scene_switch("mainmenu");
	}
}

void credits_update(float dt) {
	if (phase == 1) {
		introcard_timeout += dt;
		if (introcard_timeout > 3) {
			phase = 2;
		}
	} else if (phase == 2) {
		introcard_opacity += 400 * dt;
		if (introcard_opacity > SDL_ALPHA_OPAQUE) {
			introcard_opacity = 0;
			phase = 3;
		}
	} else if (phase == 3) {
		scroll_y -= scroll_speed * dt;
	}
}

void credits_draw(void) {
	if (phase == 1 || phase == 2) {
		draw_set_alpha(SDL_ALPHA_OPAQUE);
		draw_set_color(0x151515);
		draw_fill_rect(&RECT(0, 0,NATIVE_WIDTH, NATIVE_HEIGHT));
		draw_texture(TEX_INTRO_RACCOON, NULL, NULL);
		draw_texture(TEX_INTRO_TEXT, NULL, NULL);
		if (phase == 2) {
			draw_set_blend(true);
			draw_set_color(0x102a6e);
			draw_set_alpha((unsigned char)introcard_opacity);
			draw_fill_rect(&RECT(0, 0,NATIVE_WIDTH, NATIVE_HEIGHT));
		}
	} else {
		for (size_t i = 0; i < NUM_CREDITS_LINES; i++) {
			SDL_FRect rect = {
				/*sinf((scroll_y + i * 30) * 0.075f) * wave_amplitude*/ 0,
				scroll_y + i * 30,
				NATIVE_WIDTH, 30
			};

			if (rect.y < NATIVE_HEIGHT) {
				text_draw_shadow_centered(credits_text[i], &rect, i == 0 ? 8 : 2);
			}
		}
	}
}

Scene credits_scene = {
	"credits",
	credits_init,
	credits_event,
	credits_update,
	credits_draw,
	0x102a6e
};
