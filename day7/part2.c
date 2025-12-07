
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../vnls/vnls.h"


typedef struct {
    size_t idx;
    uint64_t timelines;
} Timeline;

typedef struct {
    Timeline *items;
    size_t length;
    size_t capacity;
} Timelines;


void append(Timelines *arr, Timeline value) {
    if (arr->capacity <= arr->length) {
        size_t new_capacity = 256;
        if (arr->capacity) {
            new_capacity = arr->capacity * 2;
        }

        arr->items = realloc(arr->items, new_capacity * sizeof(*arr->items));
        arr->capacity = new_capacity;
    }
    arr->items[arr->length++] = value;
}


size_t lookup(Timelines *arr, size_t idx) {
    for (size_t i = 0; i < arr->length; ++i) {
        Timeline t = arr->items[i];
        if (t.idx == idx) {
            return t.timelines;
        }
    }
    return 0;
}


uint64_t count(VnlS_StringBuffer *input, 
               size_t rows, 
               size_t cols, 
               Timelines *arr,
               size_t row, 
               size_t col) {
    const char HALT = 'H';
    if (row == rows) return 1;
    
    char c = input->chars[row * cols + col];
    
    if (c == HALT) {
        size_t idx = row * cols + col;
        return lookup(arr, idx);
    }
    
    if (c == '^') {
        input->chars[row * cols + col] = HALT;
        
        uint64_t timelines = count(input, rows, cols, arr, row, col - 1)
                           + count(input, rows, cols, arr, row, col + 1);
                         
        append(arr, (Timeline){
            .idx = row * cols + col,
            .timelines = timelines,
        });
        return timelines;
    } else {
        return count(input, rows, cols, arr, row + 1, col);
    }
}


uint64_t solution(VnlS_StringBuffer *input) {
    // + 1 for the last line
    size_t rows = vnls_count(input, "\n");
    size_t cols = vnls_find(input, "\n") + 1;
    size_t S = vnls_find(input, "S");
    Timelines arr = {0};
    size_t res = count(input, rows, cols, &arr, 0, S);
    free(arr.items);
    return res;
}


#define BILLION 1'000'000'000.0
#define GREEN "\033[32m"
#define RESET "\033[0m"


int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    VnlS_StringBuffer buffer = {0};
    vnls_read_file(&buffer, "input.txt");
    vnls_buf_push(&buffer, '\n');
    uint64_t count = solution(&buffer);

    clock_gettime(CLOCK_REALTIME, &end);

    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("count:\t" GREEN "%lld\n" RESET, count);
    printf("time:\t" GREEN "%f ms\n" RESET, time_spent * 1000);
    return 0;
}
