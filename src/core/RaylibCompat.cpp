#include "RaylibCompat.h"

#ifdef __MINGW32__
extern "C" {
    void sincosf(float x, float *s, float *c) {
        *s = sinf(x);
        *c = cosf(x);
    }
    int __mingw_vsscanf(const char *buf, const char *fmt, va_list ap) {
        return vsscanf(buf, fmt, ap);
    }
    static FILE* my_acrt_iob_func(unsigned index) {
        return &_iob[index];
    }
    FILE* (*_imp____acrt_iob_func)(unsigned) = my_acrt_iob_func;
}
#endif
