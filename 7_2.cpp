/*
*/

#include <iostream>
#include <cassert>
#include <climits>

#define assertm(exp, msg) assert(((void)msg, exp))

int get_byte(uint64_t number, int i) {
    return (number >> (8*i)) & 0xFF;
}

void print(uint64_t* array, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void counting_sort(uint64_t* array, int size, int byte) {
    int min_value = get_byte(array[0], byte);
    int max_value = get_byte(array[0], byte);

    for (int i = 0; i < size; i++) {
        min_value = std::min(min_value, get_byte(array[i], byte));
        max_value = std::max(max_value, get_byte(array[i], byte));
    }

    int buffer_size = max_value - min_value + 1;
    int* buffer = new int[buffer_size]();

    for (int i = 0; i < size; i++)
        buffer[get_byte(array[i], byte)-min_value]++;

    uint64_t* stable_buffer = new uint64_t[size]();

    for (int i = 1; i < buffer_size; i++) {
        buffer[i] += buffer[i-1];
    }

    for (int i = size - 1; i >= 0; i--) {
        int buffer_position = get_byte(array[i], byte) - min_value;
        buffer[buffer_position]--;
        stable_buffer[buffer[buffer_position]] = array[i];
    }

    for (int i = 0; i < size; i++) {
        array[i] = stable_buffer[i];
    }

    delete[] buffer;
    delete[] stable_buffer;
}

void lsd(uint64_t* array, int size) {
    for (int i = 0; i < 8; i++)
        counting_sort(array, size, i);
}

int main() {
    int n;
    std::cin >> n;
    assertm(n > 0 && n <= 1000000,
        "Number of elements must be between 0 and 10**6");

    uint64_t* array = new uint64_t[n];

    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
        assertm(array[i] >= 0 && array[i] <= ULLONG_MAX,
                "Array element must be between 0 and 2**64-1");
    }

    lsd(array, n);
    print(array, n);

    delete[] array;

    return 0;
}
