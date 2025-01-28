#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>/* 

 * safe_printf - async-signal-safe wrapper for printf
 */
#define MAXS 1024 

void print(const char *format, ...) {
    char buf[MAXS];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    write(1, buf, strlen(buf)); /* write is async-signal-safe */
}

void println(const char *format, ...) {
    char buf[MAXS];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    const size_t length = strlen(buf);
    buf[length] = '\n';
    write(1, buf, length + 1); /* write is async-signal-safe */
}
