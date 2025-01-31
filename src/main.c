#include <stdlib.h>
#include <stdio.h>

#include "alloc.h"
#include "fmt.h"
#include "pool.h"
#include "timer.h"
#include <string.h>
#include <time.h>

#define BUF_SIZE 10000
#define RAND_RANGE(start, end) ((long)(((double) rand() / RAND_MAX) * (end - start)) + start)

int main() {
    char buf[] = "Hello, World! This is me, Zimon!";
    const size_t size = sizeof(buf) /sizeof(char);

    start_timer();

    Pool pool = pool_init();
    char * ptr1 = pool_malloc(&pool, 1);
    memcpy(ptr1, buf, size);
    char * ptr2 = pool_malloc(&pool, size);

    memcpy(ptr2, buf, size);
    println("Buf: \"%s\"", buf);

    double execution_time = stop_timer();
    printf("Time: %.3fms\n", execution_time);
}
