#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "counting-sort.h"

static inline size_t get_key(size_t array[], size_t i) {
    return array[i];
}

DEF_COUNTING_SORT(size_t, key_sort_1, get_key)
DEF_COUNTING_SORT_M(size_t, key_sort_2, get_key);

#define MAX_KEY 100
size_t array[MAX_KEY];
size_t sorted[MAX_KEY];

void assert_sorted(size_t n, size_t array[n]) {
    for (size_t i = 0; i < n - 1; i++) {
        printf("%zu <= %zu ?\n", array[i], array[i + 1]);
        assert(array[i] <= array[i + 1]);
    }
}

void test_keys(size_t n, ...) {
    va_list args;
    va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        array[i] = va_arg(args, size_t);
    }
    va_end(args);

    size_t start, range;
    COUNTING_SORT_GET_START_AND_RANGE(array, n, get_key, start, range);
    printf("start = %zu , range = %zu\n", start, range);

    size_t *count = malloc(sizeof(size_t) * (range + 1));
    if (!count) {
        fputs("out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }

    key_sort_1(n, start, range, array, sorted, count);
    assert_sorted(n, sorted);

    if (!key_sort_2(n, array)) {
        free(count);
        fputs("out of memory\n", stderr);
    }
    assert_sorted(n, array);

    free(count);
}

int main(void) {
    test_keys(5, 3, 1, 2, 4, 0);
    test_keys(3, 0, 0, 0);
    test_keys(1, 1);

    // test already sorted
    test_keys(5, 0, 1, 2, 3, 4);

    // test keys not starting from zero
    test_keys(5, 4, 2, 3, 5, 1);

    // test keys with gaps
    test_keys(5, 6, 3, 0, 7, 9);

    // test keys with gaps and not starting from zero
    test_keys(5, 6, 3, 12, 7, 9);

    return 0;
}
