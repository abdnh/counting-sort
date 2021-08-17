#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define COUNTING_SORT_(length, start, range, array, sorted, count, GET_KEY) \
    do {                                                                    \
        memset(count, 0, sizeof(*count) * range);                           \
        for (size_t i = 0; i < length; i++) {                               \
            count[GET_KEY(array, i) - start]++;                             \
        }                                                                   \
        for (size_t i = 0; i < range - 1; i++) {                            \
            count[i + 1] += count[i];                                       \
        }                                                                   \
        for (size_t i = length - 1; i != SIZE_MAX; i--) {                   \
            size_t j = GET_KEY(array, i) - start;                           \
            count[j]--;                                                     \
            sorted[count[j]] = array[i];                                    \
        }                                                                   \
    } while (0)

// Define a counting sort function `name` that sorts an array of `T`
// using the key returned by `GET_KEY(array, i)` for each index `i`
#define DEF_COUNTING_SORT(T, name, GET_KEY)                                  \
    void name(size_t length, size_t start, size_t range,                     \
              T array[static length], T sorted[static length],               \
              size_t count[static range]) {                                  \
        COUNTING_SORT_(length, start, range, array, sorted, count, GET_KEY); \
    }

// Use to calculate start index and range if unknown
#define COUNTING_SORT_GET_START_AND_RANGE(array, length, GET_KEY, start, \
                                          range)                         \
    do {                                                                 \
        start = SIZE_MAX;                                                \
        size_t __end = 0;                                                \
        for (size_t __i = 0; __i < length; __i++) {                      \
            size_t key = GET_KEY(array, __i);                            \
            if (key < start)                                             \
                start = key;                                             \
            if (key > __end)                                             \
                __end = key;                                             \
        }                                                                \
        range = __end - start + 1;                                       \
    } while (0)

// An allocating version of `DEF_COUNTING_SORT`, with a more convenient
// interface
#define DEF_COUNTING_SORT_M(T, name, GET_KEY)                                \
    bool name(size_t length, T array[static length]) {                       \
        size_t start, range;                                                 \
        COUNTING_SORT_GET_START_AND_RANGE(array, length, GET_KEY, start,     \
                                          range);                            \
        T *sorted = malloc(sizeof(T) * length);                              \
        if (!sorted)                                                         \
            return false;                                                    \
        size_t *count = malloc(sizeof(size_t) * range);                      \
        if (!count) {                                                        \
            free(sorted);                                                    \
            return false;                                                    \
        }                                                                    \
        COUNTING_SORT_(length, start, range, array, sorted, count, GET_KEY); \
        memcpy(array, sorted, sizeof(*array) * length);                      \
        free(sorted);                                                        \
        free(count);                                                         \
        return true;                                                         \
    }
