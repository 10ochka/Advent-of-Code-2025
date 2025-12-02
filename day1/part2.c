
#include <stdio.h>
#include <time.h>
#include "../vnls/vnls.h"
#include <unistd.h>



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

        if (new_position > 0) {
            count += new_position / 100;
            new_position %= 100;
        } else if (new_position == 0) {
            count += 1;
        } else { // new_position < 0
            if (position > 0) { // encountered 0 while rotating
                count += 1;
                position = 0;
                new_position -= position;
            }
            count += -new_position / 100;
            new_position %= 100; // [-99, 0]
            new_position += 100; // [1, 100]
            new_position %= 100; // [0,  99]
        }

        position = new_position;
        vnls_chop(&input);
    }
    return count;
}



#define BILLION 1'000'000'000.0
#define GREEN "\033[32m"
#define RESET "\033[0m"



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
