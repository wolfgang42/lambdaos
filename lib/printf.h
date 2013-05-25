#include <stdarg.h>
extern int printf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
extern int sprintf(char *out, const char *format, ...) __attribute__ ((format (printf, 2, 3)));
