#pragma once

#define WRITE_LONG(data)  fwrite(&data, sizeof(uint64_t), 1, fp)
#define WRITE_INT(data)   fwrite(&data, sizeof(int), 1, fp)
#define WRITE_FLOAT(data) fwrite(&data, sizeof(float), 1, fp)
#define WRITE_CHAR(data)  fwrite(&data, sizeof(char), 1, fp)
#define WRITE_BOOL(data)  fwrite(&data, sizeof(bool), 1, fp)

#define READ_LONG(data)   fread(&data, sizeof(uint64_t), 1, fp)
#define READ_INT(data)    fread(&data, sizeof(int), 1, fp)
#define READ_FLOAT(data)  fread(&data, sizeof(float), 1, fp)
#define READ_CHAR(data)   fread(&data, sizeof(char), 1, fp)
#define READ_BOOL(data)   fread(&data, sizeof(bool), 1, fp)
