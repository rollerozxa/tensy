#include "fileio.h"
#include "consts.h"
#include <SDL3/SDL.h>
#include <string.h>

void fileio_pref_path(char *buf, size_t bufsize, const char *filename) {
	if (buf[0] != '\0')
		return;

	char *pref_path = SDL_GetPrefPath(APP_ORG, APP_NAME);
	strncpy(buf, pref_path, bufsize - 1);
	strncat(buf, filename, bufsize - 1);

	SDL_free(pref_path);
}
