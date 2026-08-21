#include "fileio.h"
#include "consts.h"
#include <SDL3/SDL.h>
#include <string.h>

bool write_float(SDL_IOStream *io, float value) {
	uint32_t bits;
	memcpy(&bits, &value, sizeof(bits));
	return SDL_WriteU32LE(io, bits);
}

bool write_double(SDL_IOStream *io, double value) {
	uint64_t bits;
	memcpy(&bits, &value, sizeof(bits));
	return SDL_WriteU64LE(io, bits);
}

bool read_float(SDL_IOStream *io, float *value) {
	uint32_t bits;
	if (!SDL_ReadU32LE(io, &bits))
		return false;

	memcpy(value, &bits, sizeof(bits));
	return true;
}

bool read_double(SDL_IOStream *io, double *value) {
	uint64_t bits;
	if (!SDL_ReadU64LE(io, &bits))
		return false;

	memcpy(value, &bits, sizeof(bits));
	return true;
}

bool read_bool(SDL_IOStream *io, bool *value) {
	uint8_t byte;
	if (!SDL_ReadU8(io, &byte))
		return false;

	*value = (byte != 0);
	return true;
}

void fileio_pref_path(char *buf, size_t bufsize, const char *filename) {
	if (buf[0] != '\0')
		return;

	char *pref_path = SDL_GetPrefPath(APP_ORG, APP_NAME);
	SDL_strlcpy(buf, pref_path, bufsize);
	SDL_strlcat(buf, filename, bufsize);

	SDL_free(pref_path);
}
