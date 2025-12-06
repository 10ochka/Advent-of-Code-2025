
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
    size_t nrows = vnls_count(input, "\n");
    
    VnlS_Partition parts = vnls_rpartition(input, "\n");
    input = parts.before;
    VnlS_String ops_input = vnls_trim(parts.after);
    
    size_t row_length = parts.after.length + 1;
    
    Array ops = {0};
    
    while (ops_input.length) {
        ops_input = vnls_ltrim(ops_input);
        char op = vnls_chop(&ops_input);
        append(&ops, op);
    }
   
    uint64_t total = 0;
    size_t problem = 0;
    Array args = {0};
    
    for (size_t col = 0; col < row_length; ++col) {
        uint64_t arg = 0;
        bool empty = true;
        for (size_t row = 0; row < nrows; ++row) {
            char c = input.chars[row * (row_length) + col];
            if (c == '\n') {
                break;
            }
            if (c != ' ') {
                arg = arg * 10 + (c - '0');
                empty = false;
            }
        }
        
        if (empty) {
            size_t result = args.items[0];
            if (ops.items[problem] == '+') {
                for (size_t i = 1; i < args.length; ++i) {
                    result += args.items[i];
                }
            } else {
                for (size_t i = 1; i < args.length; ++i) {
                    result *= args.items[i];
                }
            }  
            total += result;
            args.length = 0;
            problem++;
            continue;
        }
        append(&args, arg); 
    }
    
    free(args.items);
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
