#include "toast.h"
#include "consts.h"
#include "font.h"

Toast toast;

void show_toast(const char *message, float duration) {
	toast = (Toast){message, 2, duration, 0.0f, 1.0f, true};
}

void update_toast(float dt) {
	if (!toast.active) return;

	toast.timer += dt;
	if (toast.timer >= toast.duration) {
		toast.active = false;
		return;
	}

	// Start fading out in the last 1 second
	float remaining = toast.duration - toast.timer;
	toast.alpha = remaining < 1.0f ? remaining : 1.0f;
}

void draw_toast(SDL_Renderer *renderer) {
	if (!toast.active) return;

	SDL_FRect text_rect = calculate_text_rect(toast.message, toast.scale);
	SDL_FRect box_rect = {0, 0, text_rect.w + 24, text_rect.h + 12};

	box_rect.x = (NATIVE_WIDTH - box_rect.w) / 2;
	box_rect.y = NATIVE_HEIGHT * 0.75f - box_rect.h / 2;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 13, 13, 13, (toast.alpha * 235));
	SDL_RenderFillRect(renderer, &box_rect);

	draw_text_shadow_centered(renderer, toast.message, &box_rect, toast.scale);
}
