
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include "../vnls/vnls.h"
#include <unistd.h>

typedef struct {
    uint64_t id;
    size_t numdigits;
} NumberInfo;


NumberInfo get_id(VnlS_String *str) {
    size_t start = str->length; // length before parsing
    uint64_t id = vnls_parse_u64(str, NULL);
    size_t end = str->length; // length after parsing
    return (NumberInfo){
        .id = id,
        .numdigits = start - end,
    };
}

bool even(uint64_t id) {
    return id % 2 == 0;
}

bool odd(uint64_t id) {
    return id % 2 == 1;
}


bool is_invalid(uint64_t id, size_t numdigits) {
    if (numdigits % 2 == 1) {
        return false;
    }
    size_t repeated_part_length = numdigits / 2;
    uint64_t repeated_part_mask = pow(10, repeated_part_length);
    uint64_t repeated_hi = id / repeated_part_mask;
    uint64_t repeated_lo = id % repeated_part_mask;
    return repeated_hi == repeated_lo;
}



uint64_t solution() {
    VnlS_StringBuffer buffer = {0};
    vnls_read_file(&buffer, "input.txt");
    VnlS_String input = VnlS_String(&buffer);

    uint64_t sum = 0;

    while(input.length) {
        NumberInfo first = get_id(&input);
        vnls_chop(&input); // -
        NumberInfo last = get_id(&input);
        vnls_chop(&input); // ,

        printf("range: [%10lld, %10lld]\tdigits: %2zu -> %2zu\n", first.id, last.id, first.numdigits,  last.numdigits);

        // same magnitude
        if (first.numdigits == last.numdigits) {
            size_t numdigits = first.numdigits;

            // Odd number of digits.
            //  Since invalid IDs have even number of digits,
            //  no invalid IDs in the given range
            if (odd(numdigits)) {
                continue;
            }

            printf("\tChecking range [%lld, %lld]\n", first.id, last.id);
            for (uint64_t id = first.id; id <= last.id; ++id) {
                if (is_invalid(id, numdigits)) {
                    printf("\tInvalid ID: %lld\n", id);
                    sum += id;
                }
            }

        } else { // first.numdigits != last.numdigits
            // In this case we can ignore part of the range which
            // includes only odd number of digits.

            uint64_t subrange_first = first.id;
            uint64_t subrange_last = 0; // unknown yet

            // iterate over pairs of number lengths
            for (size_t numdigits = first.numdigits; numdigits <= last.numdigits; ++numdigits) {
                size_t next_numdigits = numdigits + 1;

                if (odd(numdigits)) {
                    continue;
                }

                subrange_first = fmax(pow(10, numdigits - 1), subrange_first);
                subrange_last = fmin(pow(10, next_numdigits - 1) - 1, last.id);

                printf("\tChecking subrange [%lld, %lld]\n", subrange_first, subrange_last);
                for (uint64_t id = subrange_first; id <= subrange_last; ++id) {
                    if (is_invalid(id, numdigits)) {
                        printf("\t\tInvalid ID: %lld\n", id);
                        sum += id;
                    }
                }

                // update subrange_first:
                // numdigits                       numdigits + 1                  numdigits + 2
                // |                               |                              |
                // [subrange_first, subrange_last] [range with odd number length] [new subrange_first, ...
                subrange_first = pow(10, numdigits + 2);
            }
        }
        // sleep(1);
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
