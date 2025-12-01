
#include <stdio.h>
#include <time.h>
#include "../vnls/vnls.h"
#include <unistd.h>


#define BILLION 1'000'000'000.0
#define GREEN "\033[32m"
#define RESET "\033[0m"



int solution() {
    VnlS_StringBuffer buffer = {0};
    vnls_read_file(&buffer, "input.txt");

    VnlS_String input = VnlS_String(&buffer);
    int position = 50;
    int count = 0;

    while (input.length) {
        char direction_letter = vnls_chop(&input);
        int direction = direction_letter == 'L'? -1 : 1;
        int num_steps = vnls_parse_i32(&input, NULL);

        int new_position = position + direction * num_steps;
        int hits = 0;

        if (new_position == 0) {
            hits = 1;
        } else if (new_position > 0) {
            hits = new_position / 100;
            new_position %= 100;
        } else {
            int zero_hit = (position == 0)? 0 : 1;
            hits = zero_hit + (-new_position / 100);
            new_position = (new_position % 100 + 100) % 100;
        }

        count += hits;
        position = new_position;
        vnls_chop(&input);
    }
    return count;
}




int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    int count = solution();

    clock_gettime(CLOCK_REALTIME, &end);

    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("count:\t" GREEN "%d\n" RESET, count);
    printf("time:\t" GREEN "%f ms\n" RESET, time_spent * 1000);
    return 0;
}
