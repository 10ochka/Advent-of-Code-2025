
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../vnls/vnls.h"


typedef struct {
    int rows;
    int cols;
    bool *data;
} Grid;


uint8_t grid_at(Grid *G, int row, int col) {
    bool outside_row = row < 0 || G->rows <= row;
    bool outside_col = col < 0 || G->cols <= col;
    if (outside_row || outside_col) {
        return 0;
    }
    return G->data[row * G->rows + col];
}


int solution(VnlS_String input) {
    int cols = vnls_find(input, "\n");
    int rows = vnls_count(input, "\n");

    Grid G = {
        .rows = rows,
        .cols = cols,
        .data = malloc(rows * cols)
    };

    // fill the grid
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char c = vnls_chop(&input);
            G.data[row * rows + col] = (c == '@');
        }
        vnls_chop(&input); // newline
    }

    const int NUMDIRS = 8;
    int dirs[][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1},
    };

    int count = 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int cell_count = 0;
            for (int dir = 0; dir < NUMDIRS; ++dir) {
                int drow = dirs[dir][0];
                int dcol = dirs[dir][1];
                cell_count += grid_at(&G, row + drow, col + dcol);
            }
            if (cell_count < 4 && grid_at(&G, row, col)) {
                count++;
            }
        }
    }

    free(G.data);
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
