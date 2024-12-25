#include <stdbool.h>

#ifndef UTIL_H
#define UTIL_H

void die(const char* fmt, ...);
bool fileinpwd(const char* file);
int cbytes(const char* file);
int clines(const char* file);
int cwords(const char* file);
int mbchars(const char* file);

#endif
