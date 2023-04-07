/*
Дано множество целых чисел из [0..10^9] размера n. 
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

Требования: к дополнительной памяти: O(n). 
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition. 
Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.
6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
*/

#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

struct IntLessComparator {
    bool operator()(const int& left, const int& right) {
        return left < right;
    }
};

template <typename T, typename Comparator>
int median_of_three(T* array, Comparator cmp, int length) {
    int left = 0;
    int middle = length/2;
    int right = length-1;

    if (!cmp(array[left], array[right]) ^ !cmp(array[left], array[middle]))
        return left;
    else if (cmp(array[middle], array[left]) ^ cmp(array[middle], array[right]))
        return middle;

    return right;
}

template <typename T, typename Comparator>
int partition(T* array, Comparator cmp, int length, int pivot) {
    std::swap(array[pivot], array[length-1]);
    int i = 0;
    for (int j = 0; j < length-1; j++) {
        if (cmp(array[j], array[length-1])) {
            std::swap(array[i], array[j]);
            i++;
        }
    }
    std::swap(array[i], array[length-1]);
    return i;
}

template <typename T, typename Comparator>
int find_stat(T* array, int length, int stat, Comparator cmp) {
    int pivot_position;
    T* buffer = new T[length];
    T value;
    std::copy(array, array+length, buffer);
    do {
        int pivot_index = median_of_three(buffer, cmp, length);
        pivot_position = partition(buffer, cmp, length, pivot_index);
        if (stat == pivot_position) {
            value = buffer[pivot_position];
            delete[] buffer;
            return value;
        }
        if (stat < pivot_position) {
            length = pivot_position;
            T* temp = new T[length];
            std::copy(buffer, buffer+length, temp);
            std::copy(temp, temp+length, buffer);
            delete[] temp;
        } else if (stat > pivot_position) {
            int old_length = length;
            length = length - (pivot_position + 1);
            stat = stat - (pivot_position + 1);
            T* temp = new T[length];
            std::copy(buffer+pivot_position+1, buffer+old_length, temp);
            std::copy(temp, temp+length, buffer);
            delete[] temp;
        }
        if (length == 1) {
            value = buffer[0];
        }
    } while (length != 1);
    delete[] buffer;
    return value;
}

int main() {
    int n;
    std::cin >> n;
    assertm(n > 0, "Array length must be greater than 0");

    int* array = new int[n];
    IntLessComparator cmp;

    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
        assertm(array[i] >= 0 && array[i] <= 1000000000,
                "Array elements must be between 0 and 10**9");
    }

    int stat_10th = n/10;
    int stat_median = n/2;
    int stat_90th = 9*n/10;

    std::cout <<
        find_stat<int, IntLessComparator>(array, n, stat_10th, cmp) <<
        std::endl;
    std::cout <<
        find_stat<int, IntLessComparator>(array, n, stat_median, cmp) <<
        std::endl;
    std::cout <<
        find_stat<int, IntLessComparator>(array, n, stat_90th, cmp)
        << std::endl;

    delete[] array;

    return 0;
}
