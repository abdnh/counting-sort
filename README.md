
A generic header-only implementation of [counting sort](https://en.wikipedia.org/wiki/Counting_sort) in C.

Counting sort is a non-negative integer sorting algorithm suitable for small integers
and is commonly used as a subroutine in [radix sort](https://en.wikipedia.org/wiki/Radix_sort).
It has linear running time thanks to the fact that it doesn't use comparisons.
It can be adapted to sort arbitrary objects as long as each object has some sort of an integer key.

## Usage

The header file `counting-sort.h` provides macros that you can use to generate
functions for each data type you want to work with.

The macros are made generic by making them take a type parameter and a `GET_KEY(T array[], size_t i)`
callback that returns an unsigned integer (`size_t`) as a key for each element in the array.

There are two macros for generating functions: `DEF_COUNTING_SORT` and `DEF_COUNTING_SORT_M`.

`DEF_COUNTING_SORT` generates a function that requires you to provide it with all the necessary memory buffers
needed by the counting sort algorithm, while `DEF_COUNTING_SORT_M` frees you from the hassle
by dynamically allocating memory and freeing it after copying the sorted data to the array originally passed.
Therefore, the two macros generate functions with different signatures.
`DEF_COUNTING_SORT_M` is intended for one-off usage or when you don't care about memory usage or performance
or just want the hassle-free way.

Here is a simplified example of using the library to sort a list of people by age:

```c
#include <stdio.h>
#include <stdlib.h>

#include "counting-sort.h"

struct person {
    const char *name;
    size_t age;
};

size_t get_key(struct person people[], size_t i) {
    // the key is simply the person's age
    return people[i].age;
}

// generate an allocating sorting function
DEF_COUNTING_SORT_M(struct person, sort_people_by_age, get_key)

int main(void) {
    struct person people[] = {
        {"Allan", 25}, {"Mary", 16}, {"Linda", 21}, {"John", 9},
        {"Ahmad", 12}, {"Eymen", 6}, {"Masha", 3},
    };
    size_t length = sizeof(people) / sizeof(people[0]);
    if (!sort_people_by_age(length, people)) {
        fputs("out of memory\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; i++) {
        printf("name = '%s' , age = %zu\n", people[i].name, people[i].age);
    }

    return 0;
}
```

Also see [demo.c](demo.c) for an example that uses both of `DEF_COUNTING_SORT` and `DEF_COUNTING_SORT_M`.

## References
- [Counting Sort on Wikipedia](https://en.wikipedia.org/wiki/Counting_sort)
- [A lecture on Coursera about counting sort by Robert Sedgewick](https://www.coursera.org/learn/algorithms-part2/lecture/2pi1Z/key-indexed-counting)
- [Visualization of counting sort](https://www.cs.usfca.edu/~galles/visualization/CountingSort.html)
