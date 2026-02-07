#include "color.h"
#include "consts.h"
#include "draw.h"
#include "gui/button.h"
#include "gui/colorpicker.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static ColorPicker color_picker;
static int active_number = -1; // 1..9 or -1 for none
static Button reset_button, save_button, cancel_button;
static unsigned int number_colors[9];

static void read_number_colors(void) {
	for (int i = 0; i < 9; i++)
		number_colors[i] = color_sdl_to_bitpack(color_numbers(i + 1));
}

void customize_colors_init(void) {
	active_number = -1;
	read_number_colors();

	BUTTON(reset_button, RECT(20, 300, 220, 40), "Reset to default");

	BUTTON(cancel_button, RECT(290, 300, 150, 40), "Cancel");

	BUTTON(save_button, RECT(460,300,150,40), "Save");
}

SDL_FPoint start = {20, 80};
SDL_FPoint gap = {100, 48};

void customize_colors_event(const SDL_Event *ev) {

	if (button_event(ev, &reset_button)) {
		color_numbers_reset();
		read_number_colors();
		active_number = -1;
		return;
	}

	if (button_event(ev, &cancel_button)) {
		scene_switch("settings");
		return;
	}

	if (button_event(ev, &save_button)) {
		for (int i = 0; i < 9; i++)
			color_set_number_color(i + 1, number_colors[i]);

		color_save_custom_numbers();

		scene_switch("settings");
		return;
	}

	if (active_number != -1 && colorpicker_event(ev, &color_picker)) {
		number_colors[active_number - 1] = color_picker.color;
		return;
	}

	// Not active: check clicks on color boxes
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		for (int i = 0; i < 9; i++) {
			int row = i / 3;
			int col = i % 3;
			if (SDL_PointInRectFloat(
					&POINT(ev->motion.x, ev->motion.y),
					&RECT(start.x + col * gap.x, start.y + row * gap.y, 40, 40))) {

				// open picker for this number
				active_number = i + 1;
				char hex[7] = {0};
				color_to_hex6(number_colors[i], hex);

				COLORPICKER(color_picker, RECT(360, start.y, 150, 150), hex);
			}
		}
	}
}

void customize_colors_update(float dt) {

}

void customize_colors_draw(void) {
	// Draw title
	text_draw_shadow("Customize number colors", 20, 20, 3);

	// Draw swatches

	for (int i = 0; i < 9; i++) {
		int row = i / 3;
		int col = i % 3;
		SDL_FRect r = RECT(start.x + col * gap.x, start.y + row * gap.y, 40, 40);

		draw_box(&r);
		draw_set_color(number_colors[i]);
		draw_fill_rect(&RECT(r.x + 2, r.y + 2, r.w - 4, r.h - 4));

		// label
		char label[8];
		snprintf(label, sizeof(label), "#%d", i + 1);
		text_draw_shadow(label, r.x + r.w + 8, r.y + 8, 2);
	}

	button(&reset_button);
	button(&save_button);
	button(&cancel_button);

	// Draw active color picker if any
	if (active_number != -1)
		colorpicker(&color_picker);
}

Scene customize_colors_scene = {
	"customize_colors",
	customize_colors_init,
	customize_colors_event,
	customize_colors_update,
	customize_colors_draw,
	0x1F3F8F
};
