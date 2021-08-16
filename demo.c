#include <stdio.h>
#include <stdlib.h>

#include "counting-sort.h"

// sort by first character
static inline size_t get_key(char *array[], size_t i) {
    return array[i][0];
}

DEF_COUNTING_SORT(char *, string_sort, get_key)

DEF_COUNTING_SORT_M(char *, string_sort_2, get_key)

void print_strings(size_t length, char *array[length]) {
    for (size_t i = 0; i < length; i++) {
        puts(array[i]);
    }
    putchar('\n');
}

#define ARRAY_LEN 4

int main(void) {
    char *array[ARRAY_LEN] = {"b", "a", "bb", "aa"};

    size_t start, range;
    COUNTING_SORT_GET_START_AND_RANGE(array, ARRAY_LEN, get_key, start, range);

    char *sorted[ARRAY_LEN];
    size_t *count = malloc(sizeof(size_t) * (range + 1));
    if (!count) {
        fputs("out of memory\n", stderr);
        return EXIT_FAILURE;
    }

    print_strings(ARRAY_LEN, array);

    string_sort(ARRAY_LEN, start, range, array, sorted, count);
    print_strings(ARRAY_LEN, sorted);

    string_sort_2(ARRAY_LEN, array);
    print_strings(ARRAY_LEN, array);

    return 0;
}
