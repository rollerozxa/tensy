#include "overlay.h"

#include <string.h>

static Overlay overlays[MAX_OVERLAYS];
static int current_overlay = -1;
static int allocated_overlays = 0;

int overlay_add(Overlay overlay) {
	if (allocated_overlays > MAX_OVERLAYS) {
		SDL_assert(0);
		return 0;
	}

	overlays[allocated_overlays] = overlay;
	allocated_overlays++;

	return 1;
}

int overlay_switch(const char *name) {
	for (size_t i = 0; i < MAX_OVERLAYS; i++) {
		if (strcmp(name, overlays[i].name) == 0) {
			current_overlay = i;
			overlay_run_init();
			return 1;
		}
	}

	SDL_assert(0);
	return 0;
}

const char *overlay_get_current(void) {
	return overlay_exists() ? overlays[current_overlay].name : NULL;
}

void overlay_hide(void) {
	current_overlay = -1;
}

bool overlay_exists(void) {
	return current_overlay != -1;
}

void overlay_run_init(void) {
	if (overlay_exists() && overlays[current_overlay].init)
		overlays[current_overlay].init();
}

void overlay_run_event(const SDL_Event *ev) {
	if (overlay_exists() && overlays[current_overlay].event)
		overlays[current_overlay].event(ev);
}

void overlay_run_update(void) {
	if (overlay_exists() && overlays[current_overlay].update)
		overlays[current_overlay].update();
}

void overlay_run_draw(void) {
	if (overlay_exists() && overlays[current_overlay].draw)
		overlays[current_overlay].draw();
}
