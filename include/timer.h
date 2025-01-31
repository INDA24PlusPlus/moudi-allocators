#pragma once

#include <sys/time.h>

#define SEC_TO_MS(sec) ((sec) * 1000LL)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec) * 1000000LL)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((double)(ns) / 1e9)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((double)(ns) / 1e6)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((double)(ns) / 1e3)

static struct timeval t_start, t_stop;

static inline void start_timer() {
    gettimeofday(&t_start, (void *) 0);
} 

static inline double stop_timer() {
    gettimeofday(&t_stop, (void *) 0);
    return SEC_TO_MS(t_stop.tv_sec - t_start.tv_sec) + (double)(t_stop.tv_usec - t_start.tv_usec) / 1000;
}
