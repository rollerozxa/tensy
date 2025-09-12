#include "fileio.h"
#include "consts.h"
#include <SDL3/SDL.h>
#include <string.h>

void fileio_pref_path(char *buf, size_t bufsize, const char *filename) {
	if (buf[0] != '\0')
		return;

	strncpy(buf, SDL_GetPrefPath(APP_ORG, APP_NAME), bufsize - 1);
	strncat(buf, filename, bufsize - 1);
}
