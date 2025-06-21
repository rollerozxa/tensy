#include "selectmode.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "gui/button.h"
#include "input.h"
#include "media/textures.h"
#include "mouse.h"
#include "render.h"
#include "media/sound.h"
#include "savestate.h"
#include "scenes/game.h"
#include "scenes/settings.h"

typedef struct {
	const char *title;
	const char *description[4];
	char board[4][7];
	bool disabled;
	void (*click)(void);
	bool (*enabled)(void);
} Mode;

#define GM_CLICK(name, enum) \
	static void name##_click(void) { \
		gamemode = enum; \
		switch_scene("gameconfig"); \
	}

GM_CLICK(classic, GM_Classic)
GM_CLICK(leisure, GM_Leisure)
GM_CLICK(five, GM_Five)

static bool five_enabled(void) {
	return settings()->secret_five;
}

static Mode modes[] = {
	{
		"Classic",
		{
			"Collect as much ",
			"score as possible",
			"within the time",
			"limit."
		},
		{
			{2,2,3,1,0,0},
			{1,2,2,2,0,0},
			{1,4,2,4,1,3},
			{2,3,2,3,4,3},
		},
		false,
		classic_click
	},
	{
		"Leisure",
		{
			"Infinite time,",
			"infinite shuffles,",
			"infinite calm."
		},
		{
			{2,2,2,2,0,0},
			{2,2,2,2,0,0},
			{2,2,1,1,1,1},
			{2,2,1,1,1,1},
		},
		false,
		leisure_click
	},
	{
		"Five",
		{
			"What?? This isn't",
			"even a real game",
			"mode!?"
		},
		{
			{5,5,5,5,5,5},
			{5,5,5,5,5,5},
			{5,5,5,5,5,5},
			{5,5,5,5,5,5},
		},
		false,
		five_click,
		five_enabled
	},
};
static size_t mode_count = sizeof(modes) / sizeof(modes[0]);

static int selected_mode = -1;
static float xoff, xvel, motion = 0;
static bool holdingdown = false;

static Button continue_button;

float lerp(float a, float b, float t) {
	return a * (1-t) + b * t;
}

void selectmode_init(void) {
	BUTTON(continue_button, RECT(NATIVE_WIDTH-180, 0, 180, 40), "Continue save");

	selected_mode = -1;
}

void selectmode_event(const SDL_Event *ev) {
	switch (ev->type) {
		case SDL_EVENT_MOUSE_WHEEL:
			xvel -= ev->wheel.y * 45;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			holdingdown = true;
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (xvel < 0.05 && selected_mode != -1) {
				sound_play(SND_CLICK);
				modes[selected_mode].click();
			}
			motion = 0;
			holdingdown = false;
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (holdingdown) {
				xvel -= ev->motion.xrel;
				motion += SDL_fabs(ev->motion.xrel);
			}
			break;

		case SDL_EVENT_KEY_UP:
			if (is_escaping(ev))
				switch_scene("mainmenu");

			break;
	}

	if (savestate_exists() && button_event(ev, &continue_button)) {
		savestate_load();
		loaded_existing = true;
		switch_scene("game");
	}
}

void selectmode_update(float dt) {
	xoff += lerp(xvel, 0, 0.75);
	xvel -= lerp(xvel, 0, 0.75);

	if (xoff < 0) {
		xoff = lerp(xoff, 0, 0.5);
	}
}

void selectmode_draw(SDL_Renderer *renderer) {
	set_font_color(CLR_WHITE);

	draw_text_shadow(renderer, "Select mode", 10, 10, 3);

	for (size_t i = 0; i < mode_count; i++) {
		set_font_color(CLR_WHITE);

		Mode mode = modes[i];

		if (mode.enabled != NULL && !mode.enabled())
			continue;

		SDL_FRect rect = {
			10 + i * 240 - xoff, 54, 230, 280
		};

		int mod = 20;
		SDL_FPoint mouse;
		int pressed = mouse_get_state_scaled(renderer, &mouse.x, &mouse.y);
		if (SDL_PointInRectFloat(&mouse, &rect) && !mode.disabled) {
			if (motion < 4) {
				if (pressed) {
					selected_mode = i;
					mod = -10;
				} else
					mod = 30;
			} else
				selected_mode = -1;
		}
		SDL_Color color = {31+mod, 63+mod, 143+mod};
		set_draw_color(renderer, sdl_color_to_bitpack(color));

		SDL_RenderFillRect(renderer, &rect);

		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 4; y++) {
				if (mode.board[y][x] == 0) continue;

				SDL_FPoint point = {
					x * 30 + rect.x + 28,
					y * 30 + rect.y + 10};

				draw_char_shadow(renderer, mode.board[y][x] + '0', point.x, point.y, 3);
			}
		}

		if (strcmp(mode.title, "Classic") == 0) {
			SDL_FRect clock_rect = {
				rect.x + 140,
				rect.y + 10,
				64, 64
			};

			SDL_RenderTexture(renderer, textures_get(TEX_CLOCK), NULL,
				&clock_rect);
		} else if (strcmp(mode.title, "Leisure") == 0) {
			SDL_FPoint z_rect = {
				rect.x + 154, rect.y + 10
			};
			draw_char_shadow(renderer, 'Z', z_rect.x, z_rect.y, 2.5);

			z_rect.x += 19;
			z_rect.y += 15;

			draw_char_shadow(renderer, 'z', z_rect.x, z_rect.y, 2.5);

			z_rect.x -= 17;
			z_rect.y += 16;

			draw_char_shadow(renderer, 'z', z_rect.x, z_rect.y, 2.5);
		}

		float centerx = (rect.w - calculate_text_rect(mode.title, 3).w) / 2;
		draw_text_shadow(renderer, mode.title, rect.x + centerx, 205, 3);

		for (int j = 0; j < 4; j++) {
			if (mode.description[j])
				draw_text_shadow(renderer, mode.description[j], rect.x + 6, 240 + j * 20, 2);
		}

		if (mode.disabled) {
			set_draw_color_alpha(renderer, 0x000000A0);
			SDL_RenderFillRect(renderer, &rect);
			const char *coming_soon = "Coming soon!";
			set_font_color((SDL_Color){0xFF, 0x00, 0x00, 0xFF});
			for (size_t i = 0; coming_soon[i] != '\0'; i++) {
				draw_char_shadow(renderer, coming_soon[i], rect.x + i * GLYPH_WIDTH * 3 + 5, rect.y + i * 10 + 5, 3);
			}
		}
	}

	if (savestate_exists())
		button(renderer, &continue_button);
}

Scene selectmode_scene = {
	"selectmode",
	selectmode_init,
	selectmode_event,
	selectmode_update,
	selectmode_draw,
	0x1F3F8F
};
