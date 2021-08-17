#include <stdio.h>
#include <stdlib.h>

#include "counting-sort.h"

// sort by first character
static inline size_t get_string_key(char *array[], size_t i) {
    return array[i][0];
}

static DEF_COUNTING_SORT(char *, string_sort, get_string_key)

static DEF_COUNTING_SORT_M(char *, string_sort_2, get_string_key)

static void print_strings(size_t length, char *array[static length]) {
    for (size_t i = 0; i < length; i++) {
        puts(array[i]);
    }
    putchar('\n');
}

#define ARRAY_LEN 4

static void sort_strings_demo(void) {
    char *array[ARRAY_LEN] = {"b", "a", "bb", "aa"};

    size_t start, range;
    COUNTING_SORT_GET_START_AND_RANGE(array, ARRAY_LEN, get_string_key, start,
                                      range);

    char *sorted[ARRAY_LEN];
    size_t *count = malloc(sizeof(size_t) * range);
    if (!count) {
        fputs("out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }

    print_strings(ARRAY_LEN, array);

    string_sort(ARRAY_LEN, start, range, array, sorted, count);
    print_strings(ARRAY_LEN, sorted);

    string_sort_2(ARRAY_LEN, array);
    print_strings(ARRAY_LEN, array);
}

struct person {
    const char *name;
    size_t age;
};

static size_t get_person_key(struct person people[], size_t i) {
    // the key is simply the person's age
    return people[i].age;
}

// generate an allocating sorting function
static DEF_COUNTING_SORT_M(struct person, sort_people_by_age, get_person_key)

static void sort_people_demo(void) {
    struct person people[] = {
        {"Allan", 25}, {"Mary", 16}, {"Linda", 21}, {"John", 9},
        {"Ahmad", 12}, {"Eymen", 6}, {"Masha", 3},
    };
    size_t length = sizeof(people) / sizeof(people[0]);
    if (!sort_people_by_age(length, people)) {
        fputs("out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < length; i++) {
        printf("name = '%s' , age = %zu\n", people[i].name, people[i].age);
    }
}

int main(void) {
    sort_strings_demo();
    sort_people_demo();

    return 0;
}
