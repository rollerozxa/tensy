#include "power.h"
#include "consts.h"
#include "draw.h"

bool power_has_battery(void) {
	int percent;
	SDL_PowerState state = SDL_GetPowerInfo(NULL, &percent);
	return percent >= 0
		|| state == SDL_POWERSTATE_ON_BATTERY
		|| state == SDL_POWERSTATE_CHARGING
		|| state == SDL_POWERSTATE_CHARGED;
}

static void power_draw_battery_outline(SDL_FRect *rect, int line) {
	float x = rect->x;
	float y = rect->y;
	float w = rect->w;
	float h = rect->h;

	// top line
	draw_fill_rect(&RECT(x, y, w + line, line));
	draw_fill_rect(&RECT(x, y, line, h + line));
	draw_fill_rect(&RECT(x + w + line, y, line, h + line));
	draw_fill_rect(&RECT(x, y + h + line, w + (line * 2), line));
	// draw the little battery nub
	draw_fill_rect(&RECT(x + w + (line * 2), y + h / 4, line, h / 2));
}

void power_draw(SDL_FPoint *pos) {

	if (!power_has_battery())
		return;

	int percent;
	SDL_PowerState state = SDL_GetPowerInfo(NULL, &percent);
	// testing all power states
	//percent = SDL_sin(SDL_GetTicks() / 1000.0) * 50 + 50;

	// Draw battery icon
	draw_set_color(0x000000);
	SDL_FRect rect = { pos->x, pos->y, 24, 12 };
	int line_width = 2;

	rect.x += 2;
	rect.y += 2;

	power_draw_battery_outline(&rect, line_width);

	draw_set_color(0xFFFFFF);

	rect.x -= 2;
	rect.y -= 2;

	power_draw_battery_outline(&rect, line_width);

	// draw a battery slice inside of the icon
	if (state == SDL_POWERSTATE_CHARGING)
		draw_set_color(0x00FF00);
	else if (percent < 15)
		draw_set_color(0xFF0000);
	else
		draw_set_color(0xFFFFFF);
	SDL_FRect slice_rect = { rect.x + line_width, rect.y + line_width, (rect.w * percent / 100), rect.h };
	draw_fill_rect(&slice_rect);
}
