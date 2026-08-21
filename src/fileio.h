#pragma once

#include "SDL3/SDL_iostream.h"
#include <stdbool.h>
#include <stddef.h>

bool write_float(SDL_IOStream *io, float value);
bool write_double(SDL_IOStream *io, double value);
bool read_float(SDL_IOStream *io, float *value);
bool read_double(SDL_IOStream *io, double *value);
bool read_bool(SDL_IOStream *io, bool *value);

#define WRITE_LONG(data)  SDL_WriteU64LE(io, data)
#define WRITE_INT(data)   SDL_WriteS32LE(io, data)
#define WRITE_UINT(data)  SDL_WriteU32LE(io, data)
#define WRITE_SHORT(data) SDL_WriteU16LE(io, data)
#define WRITE_DOUBLE(data)write_double(io, data)
#define WRITE_FLOAT(data) write_float(io, data)
#define WRITE_BYTE(data)  SDL_WriteU8(io, data)
#define WRITE_BOOL(data)  SDL_WriteU8(io, data)
#define WRITE_STRING(data, len) SDL_WriteIO(io, data, len)

#define READ_LONG(data)   SDL_ReadU64LE(io, &data)
#define READ_INT(data)    SDL_ReadS32LE(io, &data)
#define READ_UINT(data)   SDL_ReadU32LE(io, &data)
#define READ_SHORT(data)  SDL_ReadU16LE(io, &data)
#define READ_FLOAT(data)  read_float(io, &data)
#define READ_DOUBLE(data) read_double(io, &data)
#define READ_BYTE(data)   SDL_ReadU8(io, &data)
#define READ_BOOL(data)   read_bool(io, &data)
#define READ_STRING(data, len) SDL_ReadIO(io, data, len)

void fileio_pref_path(char *buf, size_t bufsize, const char *filename);
