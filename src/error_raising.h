#pragma once

#include <stdarg.h> // for va_end, va_list, va_start
#include <stdio.h>  // for fputc, stderr, and vfprintf
#include <stdlib.h> // for exit

extern inline void error(const char *format, ...) {
    va_list args; // due the ellipsis, we have to deal with "va_" stuff here

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fputc('\n', stderr);

    exit(1);
}
