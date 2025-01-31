#include <stdlib.h>
#include <stdio.h>

#include "alloc.h"
#include "fmt.h"
#include "my_allocator.h"
#include "timer.h"
#include <string.h>
#include <time.h>

#define BUF_SIZE 10000
#define MAX_MALLOCED 10000
#define RAND_RANGE(start, end) ((long)(((double) rand() / RAND_MAX) * (end - start)) + start)

void * buf[BUF_SIZE] = {0};

int main() {
    start_timer();

    for (size_t i = 0; i < BUF_SIZE; ++i) {
        println("%d", i);
        if (buf[i] == NULL) {
            buf[i] = malloc(RAND_RANGE(1, MAX_MALLOCED));
        }

        if (RAND_RANGE(1, 50) == 1) {
            size_t new_index = RAND_RANGE(i / 2, i);
            if (buf[i] == NULL) {
                continue;
            }

            i = new_index;
            free(buf[i]);
            buf[i] = NULL;
            i -= 1;
        }
    }

    for (size_t i = 0; i < BUF_SIZE; ++i) {
        println("%d) %p", i, buf[i]);
        free(buf[i]);
    }

    double execution_time = stop_timer();
    printf("Time: %.3fms\n", execution_time);
}
