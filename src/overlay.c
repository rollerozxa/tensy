#include "overlay.h"

static Overlay overlays[MAX_OVERLAYS];
static int current_overlay = -1;
static int allocated_overlays = 0;

int add_overlay(Overlay overlay) {
	if (allocated_overlays > MAX_OVERLAYS) {
		SDL_assert(0);
		return 0;
	}

	overlays[allocated_overlays] = overlay;
	allocated_overlays++;

	return 1;
}

int switch_overlay(const char *name) {
	for (size_t i = 0; i < MAX_OVERLAYS; i++) {
		if (strcmp(name, overlays[i].name) == 0) {
			current_overlay = i;
			run_overlay_init();
			return 1;
		}
	}

	SDL_assert(0);
	return 0;
}

void hide_overlay(void) {
	current_overlay = -1;
}

bool has_overlay(void) {
	return current_overlay != -1;
}

void run_overlay_init(void) {
	if (has_overlay() && overlays[current_overlay].init)
		overlays[current_overlay].init();
}

void run_overlay_event(const SDL_Event *ev) {
	if (has_overlay() && overlays[current_overlay].event)
		overlays[current_overlay].event(ev);
}

void run_overlay_update(void) {
	if (has_overlay() && overlays[current_overlay].update)
		overlays[current_overlay].update();
}

void run_overlay_draw(SDL_Renderer *renderer) {
	if (has_overlay() && overlays[current_overlay].draw)
		overlays[current_overlay].draw(renderer);
}
