#pragma once

#include <stddef.h>

#define WRITE_LONG(data)  fwrite(&data, sizeof(uint64_t), 1, fp)
#define WRITE_INT(data)   fwrite(&data, sizeof(int), 1, fp)
#define WRITE_SHORT(data) fwrite(&data, sizeof(uint16_t), 1, fp)
#define WRITE_DOUBLE(data)fwrite(&data, sizeof(double), 1, fp)
#define WRITE_FLOAT(data) fwrite(&data, sizeof(float), 1, fp)
#define WRITE_CHAR(data)  fwrite(&data, sizeof(char), 1, fp)
#define WRITE_BOOL(data)  fwrite(&data, sizeof(bool), 1, fp)
#define WRITE_STRING(data, len) fwrite(data, sizeof(char), len, fp)

#define READ_LONG(data)   fread(&data, sizeof(uint64_t), 1, fp)
#define READ_INT(data)    fread(&data, sizeof(int), 1, fp)
#define READ_SHORT(data)  fread(&data, sizeof(uint16_t), 1, fp)
#define READ_FLOAT(data)  fread(&data, sizeof(float), 1, fp)
#define READ_DOUBLE(data) fread(&data, sizeof(double), 1, fp)
#define READ_CHAR(data)   fread(&data, sizeof(char), 1, fp)
#define READ_BOOL(data)   fread(&data, sizeof(bool), 1, fp)
#define READ_STRING(data, len) fread(data, sizeof(char), len, fp)

void fileio_pref_path(char *buf, size_t bufsize, const char *filename);
