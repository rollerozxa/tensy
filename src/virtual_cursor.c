#include "virtual_cursor.h"
#include "consts.h"
#include "draw.h"
#include "gamepad.h"
#include "media/textures.h"
#include <math.h>

// Virtual cursor position
static SDL_FPoint vc = {0, 0};
static int v_clicked = 0;
static bool virtual_active = false;

// Default deadzone for joystick input
static const float JOYSTICK_DEADZONE = 0.05f;

static void clamp_virtual_pos(void) {
	vc.x = SDL_clamp(vc.x, 0, SCREEN_W);
	vc.y = SDL_clamp(vc.y, 0, SCREEN_H);
}

static void simulate_mouse_button(int button, bool down) {
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = down ? SDL_EVENT_MOUSE_BUTTON_DOWN : SDL_EVENT_MOUSE_BUTTON_UP;
	ev.button.button = button;
	ev.button.down = false;
	ev.button.x = vc.x;
	ev.button.y = vc.y;
	SDL_PushEvent(&ev);
}

static void simulate_mouse_motion(float xrel, float yrel) {
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = SDL_EVENT_MOUSE_MOTION;
	int old_x = vc.x;
	int old_y = vc.y;
	vc.x += xrel;
	vc.y += yrel;
	clamp_virtual_pos();
	ev.motion.x = vc.x;
	ev.motion.y = vc.y;
	ev.motion.xrel = vc.x - old_x;
	ev.motion.yrel = vc.y - old_y;
	SDL_PushEvent(&ev);
}

bool virtual_cursor_is_active(void) {
	return virtual_active;
}

SDL_FPoint virtual_cursor_get_pos(void) {
	return vc;
}

int virtual_cursor_clicked(void) {
	return v_clicked;
}

void virtual_cursor_event(const SDL_Event *ev) {
	if (!ev) return;

	if (!gamepad_is_connected())
		return;

	switch (ev->type) {
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if (ev->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
				v_clicked = 255;
				simulate_mouse_button(SDL_BUTTON_LEFT, true);
			}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			if (ev->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
				v_clicked = 0;
				simulate_mouse_button(SDL_BUTTON_LEFT, false);
			}
			break;
	}

}

void virtual_cursor_update(void) {
	if (!gamepad_is_connected())
		return;

	SDL_FPoint joystick = gamepad_get_left_stick();

	float dx = joystick.x;
	float dy = joystick.y;
	float dz = JOYSTICK_DEADZONE;
	if (fabsf(dx) < dz)
		dx = 0.0f;
	else
		dx = (dx > 0.0f) ? ((dx - dz) / (1.0f - dz)) : ((dx + dz) / (1.0f - dz));

	if (fabsf(dy) < dz)
		dy = 0.0f;
	else
		dy = (dy > 0.0f) ? ((dy - dz) / (1.0f - dz)) : ((dy + dz) / (1.0f - dz));

	if (!virtual_active) {
		if (dx != 0.0f || dy != 0.0f)
			virtual_active = true;
		else
			return;
	}

	simulate_mouse_motion(
		dx * 10.0f,
		dy * 10.0f);
}

void virtual_cursor_draw(void) {
	if (!virtual_active)
		return;

	SDL_FRect dstrect = {vc.x, vc.y, 16, 16};
	draw_texture(TEX_CURSOR, NULL, &dstrect);
}
