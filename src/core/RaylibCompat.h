#pragma once
#include <cmath>
#include <cstdio>
#include <cstdarg>
#include "raylib.h"

#ifdef __MINGW32__
extern "C" {
    void sincosf(float x, float *s, float *c);
    int __mingw_vsscanf(const char *buf, const char *fmt, va_list ap);
}
#endif
