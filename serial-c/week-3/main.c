#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define sea_size 30
#define movement_limit 50000
#define tests 500000
#define mines_count 10

char sea[sea_size][sea_size];

void fillMatrix(char fill_symbol) {
    for (int i = 0; i != sea_size; ++i) {
        for (int j = 0; j != sea_size; ++j) {
            sea[i][j] = fill_symbol;
        }
    }
}

void generateRandomElements(char element_symbol, int elements_count) {
  

    int x = 0;
    int y = 0;
    for (int i = 0; i != elements_count; ++i) {
        x = rand() % sea_size;
        y = rand() % sea_size;
        while (sea[x][y] == element_symbol) {
            x = rand() % sea_size;
            y = rand() % sea_size;
        }

        sea[x][y] = element_symbol;
    }
}

int movementTill(char destination_symbol, char failure_symbol) {

    int x = 0;
    int y = 0;
    for (int i = 0; i < movement_limit; ++i) {
        if (sea[x][y] == failure_symbol) return 0;
        if (sea[x][y] == destination_symbol) return 1;

        if (x == sea_size - 1) {
            ++y;
            continue;
        }
        if (y == sea_size - 1) {
            ++x;
            continue;
        }

        if (rand() % 2) {
            ++x;
        } else {
            ++y;
        }
    }

    return 0;
}

int main() {
    srand(time(NULL));  // Seed the random number generator once at the start of the program

    clock_t start_time, end_time;
    start_time = clock();

    int successes = 0;
    for (int i = 0; i != tests; ++i) {
        char water_symbol = '~';
        fillMatrix(water_symbol);

        char mine_symbol = '*';
        generateRandomElements(mine_symbol, mines_count);

        char end_symbol = 'E';
        sea[sea_size - 1][sea_size - 1] = end_symbol;

        if (movementTill(end_symbol, mine_symbol)) ++successes;
    }

    float success_probability = (float)successes / tests * 100;

    printf("Number of tests: %d\n", tests);
    printf("Number of successes: %d\n", successes);
    printf("Probability of ship reaching the end: %.2f%%\n", success_probability);

    end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Program duration: %.6f seconds\n", duration);

    return 0;
}