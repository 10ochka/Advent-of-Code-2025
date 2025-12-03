
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>
#include "../vnls/vnls.h"
#include <unistd.h>


// Get index of max digit in given range
size_t max_digit(VnlS_String range) {
    char max = range.chars[0];
    size_t idx = 0;

    for (size_t i = 0; i < range.length; ++i) {
        char curr = range.chars[i];
        if (curr > max) {
            max = curr;
            idx = i;
        }
    }
    return idx;
}


uint64_t solution(VnlS_String input) {
    const size_t NUMDIGITS = 12;

    uint64_t total_joltage = 0;
    while (input.length) {
        VnlS_Partition partition = vnls_partition(input, "\n");
        VnlS_String bank = partition.before;
        input = partition.after;

        uint64_t joltage = 0;
        size_t start = 0;
        size_t end = bank.length - NUMDIGITS + 1;

        for (size_t i = 0; i < NUMDIGITS; ++i) {
            size_t digit_idx = start + max_digit(vnls_substr(bank, start, end - start));
            uint64_t digit = bank.chars[digit_idx] - '0';
            joltage = joltage * 10 + digit;
            start = digit_idx + 1;
            end++;
        }
        total_joltage += joltage;
    }

    return total_joltage;
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
