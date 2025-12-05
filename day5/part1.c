
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../vnls/vnls.h"


typedef struct {
    uint64_t start;
    uint64_t stop;
} Range;


typedef struct {
    Range *ranges;
    size_t length;
    size_t capacity;
} Ranges;


void ranges_append(Ranges *ranges, Range r) {
    if (ranges->capacity <= ranges->length) {
        size_t new_capacity = 256;
        if (ranges->capacity) {
            new_capacity = ranges->capacity * 2;
        }

        ranges->ranges = realloc(ranges->ranges, new_capacity * sizeof(Range));
        ranges->capacity = new_capacity;
    }
    ranges->ranges[ranges->length++] = r;
}


uint64_t solution(VnlS_String input) {
    Ranges ranges = {0};

    VnlS_Partition parts = vnls_partition(input, "\n\n");
    VnlS_String ranges_input = parts.before;
    ranges_input.length++;
    VnlS_String ingredients = parts.after;

    while (ranges_input.length) {
        VnlS_Error err = 0;
        Range range;
        range.start = vnls_parse_u64(&ranges_input, &err);
        vnls_chop(&ranges_input); // -
        range.stop = vnls_parse_u64(&ranges_input, &err);
        vnls_chop(&ranges_input); // newline

        ranges_append(&ranges, range);
    }

    uint64_t count = 0;
    while (ingredients.length) {
        uint64_t id = vnls_parse_u64(&ingredients, NULL);
        vnls_chop(&ingredients); // newline

        for (size_t i = 0; i < ranges.length; ++i) {
            Range range = ranges.ranges[i];
            if (range.start <= id && id <= range.stop) {
                count++;
                break;
            }
        }
    }

    return count;
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
