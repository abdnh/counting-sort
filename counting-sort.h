#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define _COUNTING_SORT(length, start, range, array, sorted, count, GET_KEY) \
    do {                                                                    \
        memset(count, 0, sizeof(*count) * (range + 1));                     \
        for (size_t i = 0; i < length; i++) {                               \
            count[GET_KEY(array, i) - start + 1]++;                         \
        }                                                                   \
        for (size_t i = 0; i < range; i++) {                                \
            count[i + 1] += count[i];                                       \
        }                                                                   \
        for (size_t i = 0; i < length; i++) {                               \
            size_t key = GET_KEY(array, i);                                 \
            sorted[count[key - start]] = array[i];                          \
            count[key - start]++;                                           \
        }                                                                   \
    } while (0)

// Define a counting sort function `name` that sorts an array of `T`
// using the key returned by `GET_KEY(array, i)` for each index `i`
#define DEF_COUNTING_SORT(T, name, GET_KEY)                                  \
    void name(size_t length, size_t start, size_t range, T array[length],    \
              T sorted[length], size_t count[range + 1]) {                   \
        _COUNTING_SORT(length, start, range, array, sorted, count, GET_KEY); \
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

// An allocating version of `DEF_COUNTING_SORT`, with a more convenient interface
#define DEF_COUNTING_SORT_M(T, name, GET_KEY)                             \
    bool name(size_t length, T array[length]) {                              \
        size_t start, range;                                                 \
        COUNTING_SORT_GET_START_AND_RANGE(array, length, GET_KEY, start,     \
                                          range);                            \
        T *sorted = malloc(sizeof(T) * length);                              \
        if (!sorted)                                                         \
            return false;                                                    \
        size_t *count = malloc(sizeof(size_t) * (range + 1));                \
        if (!count) {                                                        \
            free(sorted);                                                    \
            return false;                                                    \
        }                                                                    \
        _COUNTING_SORT(length, start, range, array, sorted, count, GET_KEY); \
        memcpy(array, sorted, sizeof(*array) * length);                      \
        free(sorted);                                                        \
        free(count);                                                         \
        return true;                                                         \
    }
