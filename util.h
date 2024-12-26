#include <stdbool.h>

#ifndef UTIL_H
#define UTIL_H

typedef enum {
    FILE_READ,
    STR_READ
} Mode;

void die(const char* fmt, ...);
bool fileinpwd(const char* file);
int cbytes(const char* file, Mode mode);
int clines(const char* file, Mode mode);
int cwords(const char* file, Mode mode);
int cmbchars(const char* file, Mode mode);

#endif
