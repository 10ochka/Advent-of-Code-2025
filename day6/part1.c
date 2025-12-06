
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../vnls/vnls.h"


typedef struct {
    uint64_t *items;
    size_t length;
    size_t capacity;
} Array;


void append(Array *arr, uint64_t value) {
    if (arr->capacity <= arr->length) {
        size_t new_capacity = 256;
        if (arr->capacity) {
            new_capacity = arr->capacity * 2;
        }

        arr->items = realloc(arr->items, new_capacity * sizeof(uint64_t));
        arr->capacity = new_capacity;
    }
    arr->items[arr->length++] = value;
}


uint64_t solution(VnlS_String input) {
    VnlS_Partition parts = vnls_rpartition(input, "\n");
    input = vnls_trim(parts.before);
    VnlS_String ops_input = vnls_trim(parts.after);

    Array ops = {0};
    while (ops_input.length) {
        ops_input = vnls_ltrim(ops_input);
        char op = vnls_chop(&ops_input);
        append(&ops, op);
    }

    size_t nproblems = ops.length;

    uint64_t *results = malloc(nproblems * sizeof(*results));
    memset(results, 0, nproblems * sizeof(*results));

    for (size_t i = 0; i < nproblems; ++i) {
        uint64_t arg = vnls_parse_u64(&input, NULL);
        input = vnls_ltrim(input);

        results[i] = arg;
    }

    while (input.length) {
        for (size_t i = 0; i < nproblems; ++i) {
            uint64_t arg = vnls_parse_u64(&input, NULL);
            input = vnls_ltrim(input);

            if (ops.items[i] == '+') {
                results[i] += arg;
            } else {
                results[i] *= arg;
            }
        }
    }

    uint64_t total = 0;
    for (size_t i = 0; i < nproblems; ++i) {
        total += results[i];
    }

    free(ops.items);
    free(results);

    return total;
}


#define BILLION 1'000'000'000.0
#define GREEN "\033[32m"
#define RESET "\033[0m"


int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    VnlS_StringBuffer buffer = {0};
    vnls_read_file(&buffer, "input.txt");
    uint64_t count = solution(VnlS_String(&buffer));

    clock_gettime(CLOCK_REALTIME, &end);

    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("count:\t" GREEN "%lld\n" RESET, count);
    printf("time:\t" GREEN "%f ms\n" RESET, time_spent * 1000);
    return 0;
}
