
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include "../vnls/vnls.h"



int numdigits(uint64_t n) {
    int count = 0;
    if (n == 0) {
        return 1;
    }
    while (n) {
        n /= 10;
        count++;
    }
    return count;
}



bool is_invalid(uint64_t id) {
    int digits = numdigits(id);
    uint64_t orig_id = id;
    for (int divisor = 1; divisor <= digits / 2; ++divisor) {
        if (digits % divisor != 0) continue; // not a divisor

        id = orig_id;
        uint64_t mask = pow(10, divisor);
        uint64_t target_part = id % mask;
        id /= mask;
        bool matched = true;
        while (id) {
            uint64_t part = id % mask;
            id /= mask;
            if (part != target_part) {
                matched = false;
                break;
            }
        }
        if (matched) {
            return true;
        }
    }
    return false;
}



uint64_t solution() {
    VnlS_StringBuffer buffer = {0};
    vnls_read_file(&buffer, "input.txt");
    VnlS_String input = VnlS_String(&buffer);

    uint64_t sum = 0;

    while(input.length) {
        uint64_t first = vnls_parse_u64(&input, NULL);
        vnls_chop(&input); // -
        uint64_t last = vnls_parse_u64(&input, NULL);
        vnls_chop(&input); // ,

        for (uint64_t id = first; id <= last; ++id) {
            if (is_invalid(id)) {
                sum += id;
            }
        }
    }

    return sum;
}



#define BILLION 1'000'000'000.0
#define GREEN "\033[32m"
#define RESET "\033[0m"



int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    uint64_t count = solution();

    clock_gettime(CLOCK_REALTIME, &end);

    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("count:\t" GREEN "%lld\n" RESET, count);
    printf("time:\t" GREEN "%f ms\n" RESET, time_spent * 1000);
    return 0;
}
