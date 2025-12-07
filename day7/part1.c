
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../vnls/vnls.h"


uint64_t count(VnlS_StringBuffer *input, 
               size_t rows, 
               size_t cols, 
               size_t row, 
               size_t col) {
    const char HALT = 'H';
    if (row == rows) return 0;
    
    char c = input->chars[row * cols + col];
    
    if (c == HALT) return 0;
    
    if (c != '^') {
        return count(input, rows, cols, row + 1, col);
    } else {
        input->chars[row * cols + col] = HALT;
        return 1
             + count(input, rows, cols, row, col - 1)
             + count(input, rows, cols, row, col + 1);
    }
}


uint64_t solution(VnlS_StringBuffer *input) {
    // + 1 for the last line
    size_t rows = vnls_count(input, "\n");
    size_t cols = vnls_find(input, "\n") + 1;
    size_t S = vnls_find(input, "S");

    return count(input, rows, cols, 0, S);
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
