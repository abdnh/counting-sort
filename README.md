
A generic header-only implementation of [counting sort](https://en.wikipedia.org/wiki/Counting_sort) in C.

Counting sort is a simple non-negative integer sorting algorithm suitable for small integers
and is commonly used as a subroutine in [radix sort](https://en.wikipedia.org/wiki/Radix_sort).
It has linear running time thanks to the fact that it doesn't use comparisons.
It can be adapted to sort arbitrary objects as long as each object has some sort of an integer key.

The algorithm works by calculating the frequencies of each key and storing the counts in a `count` array.
Then, for each key, it computes the the position in the sorted array where the objects with that key
should be placed with the help of the count array. Finally, it passes through the input array again
and places each object in its sorted order in the output array according to the indices in the count array.

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

## Example

Here is a simplified example of using the library to sort the characters of a string lexicographically:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counting-sort.h"

size_t get_key(char *str, size_t i) {
    // the key is simply the character
    return str[i];
}

// generate the sorting function
DEF_COUNTING_SORT_M(char, sort_string, get_key)

int main(void) {
    char str[] = "the quick brown fox jumps over the lazy dog";
    printf("before: '%s'\n", str);
    sort_string(strlen(str), str);
    printf("after: '%s'\n", str);

    return 0;
}
```

Let's work through the sorting processs step by step:
1. We start by calculating the range of the keys in the input array to be able to allocate enough space for the
  count array. We also find the smallest key to be used as an offset into the count array (`count[key - offset]`) to allow sorting
  keys where the minimum key is not zero. For the input array in the above example (the string "the quick brown fox jumps over the lazy dog") we will get `offset = 32` and `range = 91`.
2. We populate the count array with the frequency of each key. We get a count array like this:
    ```
    i:        0 1 2 ...  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90
    count[i]: 8 0 0 ...   1   1   1   1   3   1   1   2   1   1   1   1   1   1   4   1   1   2   1   2   2   1   1   1   1   1
    ```
3. For each index `i` in the count array, we do the operation `count[i] += count[i - 1]` (if `i - 1` is in range).
  After this, `count[i]` will hold the index right after where the last object with key `i + offset` will be in the sorted array. Our count array will become:
    ```
    i:        0 1 2 ...  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90
    count[i]: 8 8 8 ...   9  10  11  12  15  16  17  19  20  21  22  23  24  25  29  30  31  33  34  36  38  39  40  41  42  43
    ```
4. We now go over the input array again, but this time backwards (for [stability](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability)),
  get the key of each object, store the object in its new position in the sorted array using the count array,
  then decrement its count value. In pseudocode as demonstrated in [Wikipedia](https://en.wikipedia.org/wiki/Counting_sort#Pseudocode):
    ```
    for i = length(input) - 1 downto 0 do
        j = key(input[i])
        count[j] -= 1
        output[count[j]] = input[i]
    ```
5. We're done! We now have our sorted string:
    ```
    '        abcdeeefghhijklmnoooopqrrsttuuvwxyz'
    ```

Also see [demo.c](demo.c) for an example that uses both of `DEF_COUNTING_SORT` and `DEF_COUNTING_SORT_M`.

## References
- [Counting Sort on Wikipedia](https://en.wikipedia.org/wiki/Counting_sort)
- [A lecture on Coursera about counting sort by Robert Sedgewick](https://www.coursera.org/learn/algorithms-part2/lecture/2pi1Z/key-indexed-counting)
- [Visualization of counting sort](https://www.cs.usfca.edu/~galles/visualization/CountingSort.html)
