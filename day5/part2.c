
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
    for (size_t i = 0; i < ranges->length; ++i) {
        Range *ir = &ranges->ranges[i];

        bool beg_bef = r.start < ir->start;
        bool beg_beg = r.start == ir->start;
        bool beg_mid = ir->start < r.start && r.start < ir->stop;
        bool beg_end = r.start == ir->stop;
        bool beg_aft = r.start > ir->stop;

        bool end_bef = r.stop < ir->start;
        bool end_beg = r.stop == ir->start;
        bool end_mid = ir->start < r.stop && r.stop < ir->stop;
        bool end_end = r.stop == ir->stop;
        bool end_aft = r.stop > ir->stop;

        // r :  [   ]
        // ir:         [   ]
        if (beg_bef && end_bef) {
            continue;
        }

        // r :  [   ]
        // ir:      [   ]
        if (beg_bef && end_beg) {
            r.stop = ir->start-1;
            continue;
        }

        // r :  [     ]
        // ir:     [    ]
        if (beg_bef && end_mid) {
            r.stop = ir->start-1;
            continue;
        }

        // r :  [       ]
        // ir:     [    ]
        if (beg_bef && end_end) {
            r.stop = ir->start-1;
            continue;
        }

        // r :  [         ]
        // ir:     [    ]
        if (beg_bef && end_aft) {
            ranges_append(ranges, (Range){ r.start, ir->start - 1 });
            r.start = ir->stop + 1;
            return;
        }

        // r :     []
        // ir:     [    ]
        if (beg_beg && end_beg) {
            return;
        }

        // r :     [ ]
        // ir:     [    ]
        if (beg_beg && end_mid) {
            return;
        }

        // r :     [    ]
        // ir:     [    ]
        if (beg_beg && end_end) {
            return;
        }

        // r :     [      ]
        // ir:     [    ]
        if (beg_beg && end_aft) {
            ranges_append(ranges, (Range){ ir->start + 1, r.stop });
            return;
        }

        // r :       []
        // ir:     [    ]
        if (beg_mid && end_mid) {
            return;
        }

        // r :       [  ]
        // ir:     [    ]
        if (beg_mid && end_end) {
            return;
        }

        // r :       [    ]
        // ir:     [    ]
        if (beg_mid && end_aft) {
            ranges_append(ranges, (Range){ ir->stop + 1, r.stop });
            return;
        }

        // r :         []
        // ir:     [    ]
        if (beg_end && end_end) {
            return;
        }

        // r :          [   ]
        // ir:     [    ]
        if (beg_end && end_aft) {
            ranges_append(ranges, (Range){ ir->start + 1, r.stop });
            return;
        }

        // r :            [   ]
        // ir:     [    ]
        if (beg_aft && end_aft) {
            continue;
        }
    }

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

    while (ranges_input.length) {
        VnlS_Error err = 0;
        Range range;
        range.start = vnls_parse_u64(&ranges_input, &err);
        vnls_chop(&ranges_input); // -
        range.stop = vnls_parse_u64(&ranges_input, &err);
        vnls_chop(&ranges_input); // newline

        ranges_append(&ranges, range);
    }

    size_t count = 0;
    for (size_t i = 0; i < ranges.length; ++i) {
        Range r = ranges.ranges[i];
        count += r.stop - r.start + 1;
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
